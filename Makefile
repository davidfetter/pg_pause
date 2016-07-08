# pg_pause/Makefile
MODULES = pg_pause
OBJS = pg_pause.o

EXTENSION = pg_pause
PGFILEDESC = "pg_pause - Choose whether DDL is permitted"

# Nobody should use anything but PGXS anyhow.
PGXS := $(shell pg_config --pgxs)
include $(PGXS)
