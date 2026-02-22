all:
	$(MAKE) -C demo/c

clean:
	@-$(MAKE) -C demo/c $@

.PHONY: clean gen

CIMGUI_ORG_DIR = ../cimgui
gen:
	-rm -fr libs/cimgui
	-mkdir -p libs/cimgui/imgui/backends
	-cp  $(CIMGUI_ORG_DIR)/{cimgui*.*,LICENSE,*.md,Makefile}            libs/cimgui/
	-cp  $(CIMGUI_ORG_DIR)/imgui/*.*                                    libs/cimgui/imgui/
	-cp  $(CIMGUI_ORG_DIR)/imgui/backends/imgui_impl_{glfw.*,opengl3*}  libs/cimgui/imgui/backends/
