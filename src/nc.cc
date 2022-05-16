#include <concepts>
#include <cinttypes>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

namespace NetConcepts {

class Endpoint {
 public:
   typedef struct sockaddr_in Ipv4AddressType;
   typedef struct sockaddr_in6 Ipv6AddressType;

   enum EndpointType {
     UNKNOWN = 0,
     IPV4 = PF_INET,
     IPV6 = PF_INET6,
   };

   Endpoint() : type_(UNKNOWN), addr_() {}
   Endpoint(const Ipv4AddressType& ipv4_addr) : type_(IPV4), addr_() {
     addr_.ipv4_addr = ipv4_addr;
   }
   Endpoint(const Ipv6AddressType& ipv6_addr) : type_(IPV6), addr_() {
     addr_.ipv6_addr = ipv6_addr;
   }

   EndpointType type() const { return type_; }
   Ipv4AddressType ipv4() const { return addr_.ipv4_addr; }
   Ipv6AddressType ipv6() const { return addr_.ipv6_addr; }

 private:
   EndpointType type_;

   union {
     Ipv4AddressType ipv4_addr;
     Ipv6AddressType ipv6_addr;
   } addr_;
};

template<typename T>
concept Connection = requires(T c) {
  { c.Start() } -> std::convertible_to<void>;
  { c.Stop() } -> std::convertible_to<void>;
};

template<typename T, typename C>
concept Acceptor = requires(T c) {
  { c.Open() } -> std::convertible_to<int>;
  { c.Bind(std::declval<const Endpoint&>()) } -> std::convertible_to<int>;
  { c.Listen(std::declval<int>()) } -> std::convertible_to<int>;
  { c.OnAccept(std::declval<C>(),
               std::declval<typename T::ConnectionTypeTraits::SocketType>(),
               std::declval<const Endpoint&>(),
               std::declval<const Endpoint&>()) } -> std::convertible_to<void>;
};

#if 0
template<typename T>
concept ConnectionObserver = requires(T c) {
  { c.OnDisconnect() } -> std::convertible_to<void>;
};
#endif

class ConnectionObserver {
  virtual void OnDisconnect() = 0;
};

template <Connection C>
class ConnectionTemplate : public C {
 public:
   ConnectionTemplate(ConnectionObserver* observer = nullptr) : observer_(observer_) {};

   ConnectionTemplate(ConnectionTemplate&&) = default;
   ConnectionTemplate& operator=(ConnectionTemplate&&) = default;

 private:
  ConnectionObserver* observer_;
};

class DummyObserver : public ConnectionObserver {
 public:
  void OnDisconnect() override {}
};

class BSDConnectionTypeTraits {
 public:
   typedef int SocketType;
};

class BSDConnection {
 public:
  typedef BSDConnectionTypeTraits ConnectionTypeTraits;
  void OnAccept(ConnectionTypeTraits::SocketType fd,
                const Endpoint& addr,
                const Endpoint& peer_addr) {}
  void Start();
  void Stop();

 private:
  int fd_;
};

typedef ConnectionTemplate<BSDConnection> ConnectionClass;

} // namespace NetConcepts

using namespace NetConcepts;

int main() {
  ConnectionClass c;
}
