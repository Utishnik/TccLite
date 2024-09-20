#ifndef SQLFAULTCPP
#define SQLFAULTCPP

static struct BenignMallocHooks {
  void (*xBenignBegin)(void);
  void (*xBenignEnd)(void);
} sqlite3Hooks = { 0, 0 };

void sqlite3BenignMallocHooks(
  void (*xBenignBegin)(void),
  void (*xBenignEnd)(void)
);

void sqlite3BeginBenignMalloc(void);
void sqlite3EndBenignMalloc(void);

#endif