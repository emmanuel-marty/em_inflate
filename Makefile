CC=cc
CFLAGS=-O3 -fomit-frame-pointer -Ilib -Iexample
OBJDIR=obj
LDFLAGS=

$(OBJDIR)/%.o: example/../%.c
	@mkdir -p '$(@D)'
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: lib/../%.c
	@mkdir -p '$(@D)'
	$(CC) $(CFLAGS) -c $< -o $@

APP := em_inflate

OBJS := $(OBJDIR)/example/main.o
OBJS += $(OBJDIR)/lib/em_inflate.o

all: $(APP)

$(APP): $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $(APP)

clean:
	@rm -rf $(APP) $(OBJDIR)
