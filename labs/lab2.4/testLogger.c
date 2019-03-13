#include <logger.h>

int main(){	
    initLogger("syslog");
    infof("info: %s fghi%s lmnop\n", "abcde", "jk");
    warnf("warning: %s fghi%s lmnop\n", "abcde", "jk");
    errorf("error: %s fghi%s lmnop\n", "abcde", "jk");
    panicf("panic: %s fghi%s lmnop\n", "abcde", "jk");
    initLogger("stdout");
    infof("info: %s fghi%s lmnop\n", "abcde", "jk");
    warnf("warning: %s fghi%s lmnop\n", "abcde", "jk");
    errorf("error: %s fghi%s lmnop\n", "abcde", "jk");
    panicf("panic: %s fghi%s lmnop\n", "abcde", "jk");
    return 0;
}