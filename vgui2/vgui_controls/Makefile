DLLNAME=../../lib/public/armeabi-v7a/vgui_controls.a

CXX=g++
CC=gcc
AR=ar

DLL_SRCDIR=.

DEFINES = -DVPROF_LEVEL=1 -DGNUC -DNO_HOOK_MALLOC -DNO_MALLOC_OVERRIDE -DANDROID
DEFINES += -DVPC -DRAD_TELEMETRY_DISABLED -DNDEBUG -DGNUC -DPOSIX -DCOMPILER_GCC -D_DLL_EXT=.so -D_LINUX -DLINUX -DPOSIX -D_POSIX -DDLLNAME=vinterface_cdll_wrap -DBINK_VIDEO -DGL_GLEXT_PROTOTYPES -DDX_TO_GL_ABSTRACTION -DUSE_SDL -DDEV_BUILD -DFRAME_POINTER_OMISSION_DISABLED -D_EXTERNAL_DLL_EXT=.so -DVPCGAMECAPS=VALVE -DPROJECTDIR=/home/den4ik/Desktop/srcsdk-master/vinterface_wrapper/client -D_DLL_EXT=.so -DSOURCE1=1 -DVPCGAME=valve -D_LINUX=1 -D_POSIX=1 -DLINUX=1 -DPOSIX=1

SRCS = \
    ../../public/filesystem_helpers.cpp \
    ../../vgui2/src/vgui_key_translation.cpp \
    AnalogBar.cpp \
    AnimatingImagePanel.cpp \
    AnimationController.cpp \
    BitmapImagePanel.cpp \
    BuildFactoryHelper.cpp \
    BuildGroup.cpp \
    BuildModeDialog.cpp \
    Button.cpp \
    CheckButton.cpp \
    CheckButtonList.cpp \
    CircularProgressBar.cpp \
    ComboBox.cpp \
    consoledialog.cpp \
    ControllerMap.cpp \
    controls.cpp \
    cvartogglecheckbutton.cpp \
    DirectorySelectDialog.cpp \
    Divider.cpp \
    EditablePanel.cpp \
    ExpandButton.cpp \
    FileOpenDialog.cpp \
    FileOpenStateMachine.cpp \
    FocusNavGroup.cpp \
    Frame.cpp \
    GraphPanel.cpp \
    HTML.cpp \
    Image.cpp \
    ImageList.cpp \
    ImagePanel.cpp \
    InputDialog.cpp \
    KeyBindingHelpDialog.cpp \
    KeyBoardEditorDialog.cpp \
    KeyRepeat.cpp \
    Label.cpp \
    ListPanel.cpp \
    ListViewPanel.cpp \
    Menu.cpp \
    MenuBar.cpp \
    MenuButton.cpp \
    MenuItem.cpp \
    MessageBox.cpp \
    MessageDialog.cpp \
    Panel.cpp \
    PanelListPanel.cpp \
    PerforceFileExplorer.cpp \
    PerforceFileList.cpp \
    perforcefilelistframe.cpp \
    ProgressBar.cpp \
    ProgressBox.cpp \
    PropertyDialog.cpp \
    PropertyPage.cpp \
    PropertySheet.cpp \
    QueryBox.cpp \
    RadioButton.cpp \
    RichText.cpp \
    RotatingProgressBar.cpp \
    savedocumentquery.cpp \
    ScalableImagePanel.cpp \
    ScrollableEditablePanel.cpp \
    ScrollBar.cpp \
    ScrollBarSlider.cpp \
    SectionedListPanel.cpp \
    Slider.cpp \
    Splitter.cpp \
    subrectimage.cpp \
    TextEntry.cpp \
    TextImage.cpp \
    ToggleButton.cpp \
    Tooltip.cpp \
    ToolWindow.cpp \
    TreeView.cpp \
    TreeViewListControl.cpp \
    URLLabel.cpp \
    WizardPanel.cpp \
    WizardSubPanel.cpp

SRCS_C =
OBJS = $(patsubst %.cpp,%.o,$(SRCS))
OBJS += $(patsubst %.c,%.o,$(SRCS_C))
LIBS =

OPTFLAGS= -O3 -march=armv7-a -mtune=cortex-a15 -mthumb -mfloat-abi=softfp -mfpu=neon -mcpu=cortex-a9 -pipe -mvectorize-with-neon-quad -g -fno-exceptions
CXXFLAGS=$(OPTFLAGS) -fpermissive -std=gnu++0x -Wno-narrowing
CFLAGS=$(OPTFLAGS)

INCLUDES=-I. -I../ -I../../public/ -I../../public/tier0/ -I../../public/tier1/ -I../../common/ -I../../common/GameUI/
INCLUDES += -I$(HOME)/ndk-include/cxx-stl/stlport/stlport/ -I$(HOME)/ndk-include//android/support/include/

#INCLUDES=-I. -Iinclude
LDFLAGS = -shared -march=armv7-a -mtune=cortex-a15 -mthumb -mfloat-abi=softfp -mfpu=neon -mcpu=cortex-a9 -pipe -mvectorize-with-neon-quad -fPIC -no-canonical-prefixes -Wl,--fix-cortex-a8 -Wl,--no-warn-mismatch -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now

%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) -c "$<" -o $(PWD)/"$@"

%.o : %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -c "$<" -o $(PWD)/"$@"

$(DLLNAME).so : $(OBJS)
	ar ruv $(DLLNAME) $(OBJS)
	ranlib $(DLLNAME)
clean:
	-rm -f $(OBJS)
#	-rm -f $(DLLNAME)
