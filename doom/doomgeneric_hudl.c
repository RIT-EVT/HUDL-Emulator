//#include <libc.h>
//#include "doomgeneric/doomgeneric/doomkeys.h"
//#include "doomgeneric/doomgeneric/doomgeneric.h"
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//
//#define KEYQUEUE_SIZE 16
//
//static unsigned short s_KeyQueue[KEYQUEUE_SIZE];
//static unsigned int s_KeyQueueWriteIndex = 0;
//static unsigned int s_KeyQueueReadIndex = 0;
//
//static unsigned char convertToDoomKey(unsigned int key) {
//    key = KEY_UPARROW;
//    return key;
//}
//
//static void addKeyToQueue(int pressed, unsigned int keyCode) {
//};
//
//void DG_Init() {
//
//}
//
//
//void DG_DrawFrame() {
//
//}
//
//void DG_SleepMs(uint32_t ms) {
//    usleep (ms * 1000);
//}
//
//uint32_t DG_GetTicksMs() {
//    struct timeval  tp;
//    struct timezone tzp;
//
//    gettimeofday(&tp, &tzp);
//
//    return (tp.tv_sec * 1000) + (tp.tv_usec / 1000); /* return milliseconds */
//}
//
//int DG_GetKey(int* pressed, unsigned char* doomKey) {
//    return 0;
//}
//
//void DG_SetWindowTitle(const char * title) {
//
//}
