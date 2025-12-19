#ifndef ERROR_LOG_H
#define ERROR_LOG_H
#define MAX_ERRORS 16

struct ErrorMessage {
    uint32_t time;
    char errorCode;
    int additionalData;
};

class ErrorLog {
    protected:
        int findOldestMessageIndex();
    public:
        void clean();
        void write(char code, int data);
        ErrorMessage read(uint16_t index);
        void printAll();
        ErrorLog();
};

extern ErrorLog errorLog;

#endif