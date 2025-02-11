#include "Utils.h"

NetState::NetState(bool s,Type t):QEvent(t),m_state(s) {}
