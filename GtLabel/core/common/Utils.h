#ifndef UTILS_H
#define UTILS_H

#include <boost/json.hpp>
#include <QEvent>

static void pretty_print(std::ostream& os, boost::json::value const& jv, std::string* indent = nullptr) {
    std::string indent_;
    if (!indent) indent = &indent_;
    switch (jv.kind()) {
    case boost::json::kind::object: {
        os << "{\n";
        indent->append(4, ' ');
        auto const& obj = jv.get_object();
        if (!obj.empty()) {
            auto it = obj.begin();
            for (;;) {
                os << *indent << boost::json::serialize(it->key()) << ": ";
                pretty_print(os, it->value(), indent);
                if (++it == obj.end()) break;
                os << ",\n";
            }
        }
        os << "\n";
        indent->resize(indent->size() - 4);
        os << *indent << "}";
        break;
    }

    case boost::json::kind::array: {
        os << "[\n";
        indent->append(4, ' ');
        auto const& arr = jv.get_array();
        if (!arr.empty()) {
            auto it = arr.begin();
            for (;;) {
                os << *indent;
                pretty_print(os, *it, indent);
                if (++it == arr.end()) break;
                os << ",\n";
            }
        }
        os << "\n";
        indent->resize(indent->size() - 4);
        os << *indent << "]";
    } break;

    case boost::json::kind::string:
        os << boost::json::serialize(jv.get_string());
        break;

    case boost::json::kind::uint64:
        os << jv.get_uint64();
        break;

    case boost::json::kind::int64:
        os << jv.get_int64();
        break;

    case boost::json::kind::double_:
        os << jv.get_double();
        break;

    case boost::json::kind::bool_:
        if (jv.get_bool())
            os << "true";
        else
            os << "false";
        break;

    case boost::json::kind::null:
        os << "null";
        break;
    }

    if (indent->empty()) os << "\n";
}

class NetState:public QEvent{
public:
    explicit NetState(bool s,Type t = TYPE);
    static const Type TYPE = static_cast<QEvent::Type>(Type::User+1001);
    inline bool state(){
        return m_state;
    }
private:
    bool m_state;
};

namespace common {
    inline std::string username;
    inline std::uint32_t userid;
    inline std::string serveralgoIP;
    inline std::string serveralgoPort;

    inline std::string server1IP;
    inline std::string server1Port;

    inline std::string server2IP;
    inline std::string server2Port;

    inline std::string grianType;
    inline std::string tags;
}


#endif
