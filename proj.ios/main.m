#import <UIKit/UIKit.h>

// Under iOS and the Simulator, we can use an alternate Accelerometer interface
#import "AccelerometerSimulation.h"

// clean the console output.
typedef int (*PYStdWriter)(void *, const char *, int);
static PYStdWriter _oldStdWrite;
int __pyStderrWrite(void *inFD, const char *buffer, int size)
{
    if (strncmp(buffer, "AssertMacros:", 13) == 0) {
        return 0;
    }
    return _oldStdWrite(inFD, buffer, size);
}
void __iOS7B5CleanConsoleOutput(void)
{
    _oldStdWrite = stderr->_write;
    stderr->_write = __pyStderrWrite;
}

int main(int argc, char *argv[]) {
    __iOS7B5CleanConsoleOutput();
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, @"AppController");
    [pool release];
    return retVal;
}



