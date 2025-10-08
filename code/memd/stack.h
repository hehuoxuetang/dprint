#ifndef _STACKD_H
#define _STACKD_H

//------------------------------EXTERN_FUNCTION_START--------------------------------//

// 栈初始化的默认值
#define STACK_INIT_VAL	(0xcc)

int Stack_StartCount(int vMaxSize);
int Stack_EndCount(void);

int Stack_Check(unsigned char *szStack, int vSize);

#define STACK_CHECK_SIZE	4
#define STACK_CHECK_S	{unsigned char szStack[STACK_CHECK_SIZE] = {STACK_INIT_VAL, STACK_INIT_VAL, STACK_INIT_VAL, STACK_INIT_VAL};
#define STACK_CHECK_E	Stack_Check(szStack, STACK_CHECK_SIZE);
#define STACK_CHECK_P	Stack_Check(szStack, STACK_CHECK_SIZE);
#define STACK_CHECK_F	}

//------------------------------EXTERN_FUNCTION_END--------------------------------//

#endif
