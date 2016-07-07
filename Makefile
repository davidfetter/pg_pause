# pg_pause/Makefile

MODULE_big = pg_pause
OBJS = pg_pause.o

# Nobody should use anything but PGXS anyhow.
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
