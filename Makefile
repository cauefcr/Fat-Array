OBJ = src/test
EXT = .c

#CC specifies which compiler we're using
CC = clang

#all clang specific
MOST = -Wextra -Wparentheses -Wswitch -Wswitch-bool -Wcast-of-sel-type -Wchar-subscripts -Wcomment -Wdelete-non-virtual-dtor -Wextern-c-compat -Wfor-loop-analysis -Wformat -Wimplicit -Winfinite-recursion -Wmismatched-tags -Wmissing-braces -Wmove -Wmultichar -Wobjc-designated-initializers -Wobjc-missing-super-calls -Woverloaded-virtual -Wprivate-extern -Wreorder -Wreturn-type -Wself-assign -Wself-move -Wsizeof-array-argument -Wsizeof-array-decay -Wstring-plus-int -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wunused-argument -Wunused-label -Wunused-local-typedef -Wunused-private-field -Wunused-property-ivar -Wunused-value -Wunused-variable -Wvolatile-register-var
#missing -Wunused-functions

GCC_MOST = -Wall -Wextra 

PEDANTIC = -Wc++11-long-long -Wc++14-binary-literal -Wc11-extensions -Wcomplex-component-init -Wdeclaration-after-statement -Wdollar-in-identifier-extension -Wembedded-directive -Wempty-translation-unit -Wextended-offsetof -Wflexible-array-extensions -Wformat-pedantic -Wfour-char-constants -Wgnu-anonymous-struct -Wgnu-auto-type -Wgnu-binary-literal -Wgnu-case-range -Wgnu-complex-integer -Wgnu-compound-literal-initializer -Wgnu-conditional-omitted-operand -Wgnu-empty-initializer -Wgnu-empty-struct -Wgnu-flexible-array-initializer -Wgnu-flexible-array-union-member -Wgnu-folding-constant -Wgnu-imaginary-constant -Wgnu-include-next -Wgnu-label-as-value -Wgnu-redeclared-enum -Wgnu-statement-expression -Wgnu-union-cast -Wgnu-zero-line-directive -Wgnu-zero-variadic-macro-arguments -Wimport-preprocessor-directive-pedantic -Wkeyword-macro -Wlanguage-extension-token -Wlong-long -Wmicrosoft-charize -Wmicrosoft-comment-paste -Wmicrosoft-cpp-macro -Wmicrosoft-end-of-file -Wmicrosoft-enum-forward-reference -Wmicrosoft-fixed-enum -Wmicrosoft-flexible-array -Wmicrosoft-redeclare-static -Wnested-anon-types -Wnullability-extension -Woverlength-strings -Wretained-language-linkage -Wvariadic-macros -Wvla-extension -Wzero-length-array
#missing -Wextra-semi

GCC_PEDANTIC = -Wpedantic

COMPILER_FLAGS = -std=c99 $(MOST) $(PEDANTIC) -g

COMPILER_FLAGS_GCC = -std=c99 $(GCC_MOST) $(GCC_PEDANTIC) -g

# -DDEBUG
LINKER_FLAGS = -lm
OBJ_NAME = test

all: objects

objects:
	$(CC) $(OBJ)$(EXT) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ).o
	mv $(OBJ).o $(OBJ_NAME).o

macro:
	cpp $(OBJ)$(EXT) | clang-format -style=llvm | grep -E -v '#.*' | clang-format -style=llvm > $(OBJ_NAME)_pp.c

clean:
	rm $(OBJ_NAME).o $(OBJ)_pp.c

gcc:
	gcc $(OBJ)$(EXT) $(COMPILER_FLAGS_GCC) $(LINKER_FLAGS) -o $(OBJ).o
	mv $(OBJ).o $(OBJ_NAME).o