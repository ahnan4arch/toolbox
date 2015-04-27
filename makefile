all: toolbox.a

.PHONY : clean test watch
.DELETE_ON_ERROR : $(wildcard objs/*)


####################################################################### VALUES

COPTS += -Wall -Werror -IX11 -I.
CCOPTS += $(COPTS)


ssfc = .c .cc .glsl
spth = */* */*/*
srcs = $(foreach p, $(spth), $(foreach s, $(ssfc), $(wildcard $(p)$(s))))
hdrs = $(foreach p, * $(spth), $(foreach s, .h, $(wildcard $(p)$(s))))

dirs = $(sort $(dir $(srcs)))
mods = $(basename $(notdir $(srcs)))
dmds= $(addprefix objs/, $(mods))
objs = $(addsuffix .o, $(dmds)) $(exobjs)
deps = $(addsuffix .d, $(dmds))

exobjs =



######################################################################## RULES

toolbox.a: makefile $(objs)
	ar rc $@ $(objs)

clean:
	rm -f objs/* toolbox*



################################################################# COMMON RULES


-include $(deps)

vpath %.o objs
vpath % $(dirs)


objs/%.o : %.cc makefile
	$(CC) $(CCOPTS) -c -o $@ $<

objs/%.o : %.c makefile
	${CC} $(COPTS) -c -o $@ $<

objs/%.o : %.glsl makefile
	objcopy -I binary -O elf64-x86-64 -B i386 $< $@

objs/%.d : %.cc
	@echo $@
	@echo -n objs/ > $@
	@$(CPP) $(CCOPTS) -MM $< >> $@

objs/%.d : %.c
	@echo $@
	@echo -n objs/ > $@
	@$(CPP) $(COPTS) -MM $< >> $@


