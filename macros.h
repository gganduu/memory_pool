#ifndef MACROS_H_
#define MACROS_H_

#include <sstream>

namespace zz{
class LogFatal{
private:
    struct Entry{
        std::string file_name_;
        int line_num_;
        std::ostringstream ss_;
        void Init(const std::string& file_name, int line_num){
            this->file_name_ = file_name;
            this->line_num_ = line_num;
            this->ss_.str("");
        }
    };

public:
    LogFatal(const std::string& file_name, int line_num){
        GetEntry().Init(file_name, line_num);
    }
    static Entry& GetEntry();
    std::ostringstream& GetStream(){
        return GetEntry().ss_;
    }
};

inline LogFatal::Entry& LogFatal::GetEntry(){
    static thread_local LogFatal::Entry entry;
    return entry;
}

} // namespace zz

#define UNUSED(x) (void)(x)
#define PREDICT_FALSE(x) (__builtin_expect(!!(x), 0))
#define STRINGFY(x) #x
#define DISABLE_COPY_AND_ASSIGN(TYPENAME)         \
        TYPENAME(const TYPENAME&) = delete;       \
        void operator=(const TYPENAME&) = delete

#define CHECK_BINARY_OP(op, a, b)                                                                            \
    do {                                                                                                     \
        if (!((a) op (b))) {                                                                                 \
            zz::LogFatal(__FILE__, __LINE__).GetStream() << "Check Failed " << #a << " " << #op << " " << #b \
            << " at " << __FILE__ << ":" << __LINE__;                                                        \
            std::abort();                                                                                    \
        }                                                                                                    \
    }while(0);

#define CHECK_EQ(a, b) CHECK_BINARY_OP(==, a, b)
#define CHECK_NE(a, b) CHECK_BINARY_OP(!=, a, b)
#define CHECK_GE(a, b) CHECK_BINARY_OP(>=, a, b)
#define CHECK_GT(a, b) CHECK_BINARY_OP(>, a, b)
#define CHECK_LE(a, b) CHECK_BINARY_OP(<=, a, b)
#define CHECK_LT(a, b) CHECK_BINARY_OP(<, a, b)

#define CHECK(expr)                                                                                 \
    do{                                                                                             \
        if (!(expr)) {                                                                              \
            zz::LogFatal(__FILE__, __LINE__).GetStream() << "Check failed: " #expr                  \
            << " at " << __FILE__ << ":" << __LINE__;                                               \
            std::abort();                                                                           \
        }                                                                                           \
    } while(0);

#endif