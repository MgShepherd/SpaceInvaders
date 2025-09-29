#ifndef _M_STATE_H_
#define _M_STATE_H_

typedef enum {
  M_MENU,
  M_GAME
} M_State;

void m_app_run();

void m_app_state_change(M_State new_state);

#endif // _M_STATE_H_
