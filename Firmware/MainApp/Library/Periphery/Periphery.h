#ifndef PERIPHERY_H
#define PERIPHERY_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Task for updating universal inputs periodically
 * @param parameters Task parameters (unused)
 */
void inputUpdateTask(void *parameters);

#ifdef __cplusplus
}
#endif

#endif // PERIPHERY_H

