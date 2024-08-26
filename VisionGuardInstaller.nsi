############################################################################################
#      NSIS Installation Script created by NSIS Quick Setup Script Generator v1.09.18
#               Entirely Edited with NullSoft Scriptable Installation System                
#              by Vlasis K. Barkas aka Red Wine red_wine@freemail.gr Sep 2006               
############################################################################################

!define APP_NAME "VisionGuard"
!define COMP_NAME "GSoC-24 Intel OpenVINO Toolkit"
!define WEB_SITE "https://summerofcode.withgoogle.com/programs/2024/projects/QUbIeRAM"
!define VERSION "1.1.0.0"
!define COPYRIGHT "Inbasekaran Perumal  © 2024"
!define DESCRIPTION "Application"
!define LICENSE_TXT "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\License.txt.txt"
!define INSTALLER_NAME "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Setup-Windows-x64\VisionGuard-Setup.exe"
!define MAIN_APP_EXE "VisionGuardApp.exe"
!define INSTALL_TYPE "SetShellVarContext current"
!define REG_ROOT "HKCU"
!define REG_APP_PATH "Software\Microsoft\Windows\CurrentVersion\App Paths\${MAIN_APP_EXE}"
!define UNINSTALL_PATH "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"

!define REG_START_MENU "Start Menu Folder"

var SM_Folder

######################################################################

VIProductVersion  "${VERSION}"
VIAddVersionKey "ProductName"  "${APP_NAME}"
VIAddVersionKey "CompanyName"  "${COMP_NAME}"
VIAddVersionKey "LegalCopyright"  "${COPYRIGHT}"
VIAddVersionKey "FileDescription"  "${DESCRIPTION}"
VIAddVersionKey "FileVersion"  "${VERSION}"

######################################################################

SetCompressor ZLIB
Name "${APP_NAME}"
Caption "${APP_NAME}"
OutFile "${INSTALLER_NAME}"
BrandingText "${APP_NAME}"
XPStyle on
InstallDirRegKey "${REG_ROOT}" "${REG_APP_PATH}" ""
InstallDir "$PROGRAMFILES\VisionGuard"

######################################################################

!include "MUI.nsh"

!define MUI_ABORTWARNING
!define MUI_UNABORTWARNING

!insertmacro MUI_PAGE_WELCOME

!ifdef LICENSE_TXT
!insertmacro MUI_PAGE_LICENSE "${LICENSE_TXT}"
!endif

!insertmacro MUI_PAGE_DIRECTORY

!ifdef REG_START_MENU
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "VisionGuard"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${REG_ROOT}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${UNINSTALL_PATH}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${REG_START_MENU}"
!insertmacro MUI_PAGE_STARTMENU Application $SM_Folder
!endif

!insertmacro MUI_PAGE_INSTFILES

!define MUI_FINISHPAGE_RUN "$INSTDIR\${MAIN_APP_EXE}"
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM

!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

######################################################################

Section -MainProgram
${INSTALL_TYPE}
SetOverwrite ifnewer
SetOutPath "$INSTDIR"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\D3Dcompiler_47.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\License.txt.txt"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\opencv_videoio_ffmpeg4100_64.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\opencv_videoio_msmf4100_64.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\opencv_videoio_msmf4100_64d.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\opencv_world4100.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\opencv_world4100d.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\opengl32sw.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\openvino.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\openvino_auto_batch_plugin.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\openvino_auto_plugin.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\openvino_c.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\openvino_hetero_plugin.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\openvino_intel_cpu_plugin.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\openvino_intel_gpu_plugin.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\openvino_intel_npu_plugin.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\openvino_ir_frontend.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\openvino_onnx_frontend.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\openvino_paddle_frontend.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\openvino_pytorch_frontend.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\openvino_tensorflow_frontend.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\openvino_tensorflow_lite_frontend.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\Qt6Charts.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\Qt6Core.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\Qt6Gui.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\Qt6Network.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\Qt6OpenGL.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\Qt6OpenGLWidgets.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\Qt6Pdf.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\Qt6Svg.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\Qt6Widgets.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\tbb12.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\vision-guard-removebg.png"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\VisionGuardApp.exe"
SetOutPath "$INSTDIR\translations"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\translations\qt_ar.qm"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\translations\qt_bg.qm"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\translations\qt_ca.qm"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\translations\qt_cs.qm"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\translations\qt_da.qm"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\translations\qt_de.qm"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\translations\qt_en.qm"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\translations\qt_es.qm"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\translations\qt_fa.qm"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\translations\qt_fi.qm"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\translations\qt_fr.qm"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\translations\qt_gd.qm"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\translations\qt_he.qm"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\translations\qt_hr.qm"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\translations\qt_hu.qm"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\translations\qt_it.qm"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\translations\qt_ja.qm"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\translations\qt_ka.qm"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\translations\qt_ko.qm"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\translations\qt_lv.qm"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\translations\qt_nl.qm"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\translations\qt_nn.qm"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\translations\qt_pl.qm"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\translations\qt_pt_BR.qm"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\translations\qt_ru.qm"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\translations\qt_sk.qm"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\translations\qt_tr.qm"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\translations\qt_uk.qm"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\translations\qt_zh_CN.qm"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\translations\qt_zh_TW.qm"
SetOutPath "$INSTDIR\tls"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\tls\qcertonlybackend.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\tls\qopensslbackend.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\tls\qschannelbackend.dll"
SetOutPath "$INSTDIR\styles"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\styles\qmodernwindowsstyle.dll"
SetOutPath "$INSTDIR\platforms"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\platforms\qwindows.dll"
SetOutPath "$INSTDIR\omz_models\open-closed-eye-0001"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\open-closed-eye-0001\open-closed-eye.onnx"
SetOutPath "$INSTDIR\omz_models\open-closed-eye-0001\FP32"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\open-closed-eye-0001\FP32\open-closed-eye-0001.bin"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\open-closed-eye-0001\FP32\open-closed-eye-0001.xml"
SetOutPath "$INSTDIR\omz_models\open-closed-eye-0001\FP16-INT8"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\open-closed-eye-0001\FP16-INT8\open-closed-eye-0001.bin"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\open-closed-eye-0001\FP16-INT8\open-closed-eye-0001.xml"
SetOutPath "$INSTDIR\omz_models\open-closed-eye-0001\FP16"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\open-closed-eye-0001\FP16\open-closed-eye-0001.bin"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\open-closed-eye-0001\FP16\open-closed-eye-0001.xml"
SetOutPath "$INSTDIR\omz_models\head-pose-estimation-adas-0001\FP32"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\head-pose-estimation-adas-0001\FP32\head-pose-estimation-adas-0001.bin"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\head-pose-estimation-adas-0001\FP32\head-pose-estimation-adas-0001.xml"
SetOutPath "$INSTDIR\omz_models\head-pose-estimation-adas-0001\FP16-INT8"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\head-pose-estimation-adas-0001\FP16-INT8\head-pose-estimation-adas-0001.bin"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\head-pose-estimation-adas-0001\FP16-INT8\head-pose-estimation-adas-0001.xml"
SetOutPath "$INSTDIR\omz_models\head-pose-estimation-adas-0001\FP16"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\head-pose-estimation-adas-0001\FP16\head-pose-estimation-adas-0001.bin"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\head-pose-estimation-adas-0001\FP16\head-pose-estimation-adas-0001.xml"
SetOutPath "$INSTDIR\omz_models\gaze-estimation-adas-0002\FP32"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\gaze-estimation-adas-0002\FP32\gaze-estimation-adas-0002.bin"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\gaze-estimation-adas-0002\FP32\gaze-estimation-adas-0002.xml"
SetOutPath "$INSTDIR\omz_models\gaze-estimation-adas-0002\FP16-INT8"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\gaze-estimation-adas-0002\FP16-INT8\gaze-estimation-adas-0002.bin"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\gaze-estimation-adas-0002\FP16-INT8\gaze-estimation-adas-0002.xml"
SetOutPath "$INSTDIR\omz_models\gaze-estimation-adas-0002\FP16"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\gaze-estimation-adas-0002\FP16\gaze-estimation-adas-0002.bin"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\gaze-estimation-adas-0002\FP16\gaze-estimation-adas-0002.xml"
SetOutPath "$INSTDIR\omz_models\facial-landmarks-98-detection-0001\FP32"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\facial-landmarks-98-detection-0001\FP32\facial-landmarks-98-detection-0001.bin"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\facial-landmarks-98-detection-0001\FP32\facial-landmarks-98-detection-0001.xml"
SetOutPath "$INSTDIR\omz_models\facial-landmarks-98-detection-0001\FP16-INT8"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\facial-landmarks-98-detection-0001\FP16-INT8\facial-landmarks-98-detection-0001.bin"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\facial-landmarks-98-detection-0001\FP16-INT8\facial-landmarks-98-detection-0001.xml"
SetOutPath "$INSTDIR\omz_models\facial-landmarks-98-detection-0001\FP16"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\facial-landmarks-98-detection-0001\FP16\facial-landmarks-98-detection-0001.bin"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\facial-landmarks-98-detection-0001\FP16\facial-landmarks-98-detection-0001.xml"
SetOutPath "$INSTDIR\omz_models\facial-landmarks-35-adas-0002\FP32"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\facial-landmarks-35-adas-0002\FP32\facial-landmarks-35-adas-0002.bin"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\facial-landmarks-35-adas-0002\FP32\facial-landmarks-35-adas-0002.xml"
SetOutPath "$INSTDIR\omz_models\facial-landmarks-35-adas-0002\FP16-INT8"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\facial-landmarks-35-adas-0002\FP16-INT8\facial-landmarks-35-adas-0002.bin"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\facial-landmarks-35-adas-0002\FP16-INT8\facial-landmarks-35-adas-0002.xml"
SetOutPath "$INSTDIR\omz_models\facial-landmarks-35-adas-0002\FP16"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\facial-landmarks-35-adas-0002\FP16\facial-landmarks-35-adas-0002.bin"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\facial-landmarks-35-adas-0002\FP16\facial-landmarks-35-adas-0002.xml"
SetOutPath "$INSTDIR\omz_models\face-detection-retail-0005\FP32"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\face-detection-retail-0005\FP32\face-detection-retail-0005.bin"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\face-detection-retail-0005\FP32\face-detection-retail-0005.xml"
SetOutPath "$INSTDIR\omz_models\face-detection-retail-0005\FP16-INT8"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\face-detection-retail-0005\FP16-INT8\face-detection-retail-0005.bin"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\face-detection-retail-0005\FP16-INT8\face-detection-retail-0005.xml"
SetOutPath "$INSTDIR\omz_models\face-detection-retail-0005\FP16"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\face-detection-retail-0005\FP16\face-detection-retail-0005.bin"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\face-detection-retail-0005\FP16\face-detection-retail-0005.xml"
SetOutPath "$INSTDIR\omz_models\face-detection-retail-0004\FP32"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\face-detection-retail-0004\FP32\face-detection-retail-0004.bin"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\face-detection-retail-0004\FP32\face-detection-retail-0004.xml"
SetOutPath "$INSTDIR\omz_models\face-detection-retail-0004\FP16-INT8"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\face-detection-retail-0004\FP16-INT8\face-detection-retail-0004.bin"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\face-detection-retail-0004\FP16-INT8\face-detection-retail-0004.xml"
SetOutPath "$INSTDIR\omz_models\face-detection-retail-0004\FP16"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\face-detection-retail-0004\FP16\face-detection-retail-0004.bin"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\face-detection-retail-0004\FP16\face-detection-retail-0004.xml"
SetOutPath "$INSTDIR\omz_models\face-detection-adas-0001\FP32"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\face-detection-adas-0001\FP32\face-detection-adas-0001.bin"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\face-detection-adas-0001\FP32\face-detection-adas-0001.xml"
SetOutPath "$INSTDIR\omz_models\face-detection-adas-0001\FP16-INT8"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\face-detection-adas-0001\FP16-INT8\face-detection-adas-0001.bin"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\face-detection-adas-0001\FP16-INT8\face-detection-adas-0001.xml"
SetOutPath "$INSTDIR\omz_models\face-detection-adas-0001\FP16"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\face-detection-adas-0001\FP16\face-detection-adas-0001.bin"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\omz_models\face-detection-adas-0001\FP16\face-detection-adas-0001.xml"
SetOutPath "$INSTDIR\networkinformation"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\networkinformation\qnetworklistmanager.dll"
SetOutPath "$INSTDIR\imageformats"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\imageformats\qgif.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\imageformats\qicns.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\imageformats\qico.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\imageformats\qjpeg.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\imageformats\qpdf.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\imageformats\qsvg.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\imageformats\qtga.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\imageformats\qtiff.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\imageformats\qwbmp.dll"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\imageformats\qwebp.dll"
SetOutPath "$INSTDIR\iconengines"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\iconengines\qsvgicon.dll"
SetOutPath "$INSTDIR\generic"
File "C:\Users\Inba\Documents\GSoC\VisionGuard\build\bin\Vision-Guard-Windows-x64\generic\qtuiotouchplugin.dll"
SectionEnd

######################################################################

Section -Icons_Reg
SetOutPath "$INSTDIR"
WriteUninstaller "$INSTDIR\uninstall.exe"

!ifdef REG_START_MENU
!insertmacro MUI_STARTMENU_WRITE_BEGIN Application
CreateDirectory "$SMPROGRAMS\$SM_Folder"
CreateShortCut "$SMPROGRAMS\$SM_Folder\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$SMPROGRAMS\$SM_Folder\Uninstall ${APP_NAME}.lnk" "$INSTDIR\uninstall.exe"

!ifdef WEB_SITE
WriteIniStr "$INSTDIR\${APP_NAME} website.url" "InternetShortcut" "URL" "${WEB_SITE}"
CreateShortCut "$SMPROGRAMS\$SM_Folder\${APP_NAME} Website.lnk" "$INSTDIR\${APP_NAME} website.url"
!endif
!insertmacro MUI_STARTMENU_WRITE_END
!endif

!ifndef REG_START_MENU
CreateDirectory "$SMPROGRAMS\VisionGuard"
CreateShortCut "$SMPROGRAMS\VisionGuard\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$SMPROGRAMS\VisionGuard\Uninstall ${APP_NAME}.lnk" "$INSTDIR\uninstall.exe"

!ifdef WEB_SITE
WriteIniStr "$INSTDIR\${APP_NAME} website.url" "InternetShortcut" "URL" "${WEB_SITE}"
CreateShortCut "$SMPROGRAMS\VisionGuard\${APP_NAME} Website.lnk" "$INSTDIR\${APP_NAME} website.url"
!endif
!endif

WriteRegStr ${REG_ROOT} "${REG_APP_PATH}" "" "$INSTDIR\${MAIN_APP_EXE}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayName" "${APP_NAME}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "UninstallString" "$INSTDIR\uninstall.exe"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayIcon" "$INSTDIR\${MAIN_APP_EXE}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayVersion" "${VERSION}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "Publisher" "${COMP_NAME}"

!ifdef WEB_SITE
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "URLInfoAbout" "${WEB_SITE}"
!endif
SectionEnd

######################################################################

Section Uninstall
${INSTALL_TYPE}
Delete "$INSTDIR\D3Dcompiler_47.dll"
Delete "$INSTDIR\License.txt.txt"
Delete "$INSTDIR\opencv_videoio_ffmpeg4100_64.dll"
Delete "$INSTDIR\opencv_videoio_msmf4100_64.dll"
Delete "$INSTDIR\opencv_videoio_msmf4100_64d.dll"
Delete "$INSTDIR\opencv_world4100.dll"
Delete "$INSTDIR\opencv_world4100d.dll"
Delete "$INSTDIR\opengl32sw.dll"
Delete "$INSTDIR\openvino.dll"
Delete "$INSTDIR\openvino_auto_batch_plugin.dll"
Delete "$INSTDIR\openvino_auto_plugin.dll"
Delete "$INSTDIR\openvino_c.dll"
Delete "$INSTDIR\openvino_hetero_plugin.dll"
Delete "$INSTDIR\openvino_intel_cpu_plugin.dll"
Delete "$INSTDIR\openvino_intel_gpu_plugin.dll"
Delete "$INSTDIR\openvino_intel_npu_plugin.dll"
Delete "$INSTDIR\openvino_ir_frontend.dll"
Delete "$INSTDIR\openvino_onnx_frontend.dll"
Delete "$INSTDIR\openvino_paddle_frontend.dll"
Delete "$INSTDIR\openvino_pytorch_frontend.dll"
Delete "$INSTDIR\openvino_tensorflow_frontend.dll"
Delete "$INSTDIR\openvino_tensorflow_lite_frontend.dll"
Delete "$INSTDIR\Qt6Charts.dll"
Delete "$INSTDIR\Qt6Core.dll"
Delete "$INSTDIR\Qt6Gui.dll"
Delete "$INSTDIR\Qt6Network.dll"
Delete "$INSTDIR\Qt6OpenGL.dll"
Delete "$INSTDIR\Qt6OpenGLWidgets.dll"
Delete "$INSTDIR\Qt6Pdf.dll"
Delete "$INSTDIR\Qt6Svg.dll"
Delete "$INSTDIR\Qt6Widgets.dll"
Delete "$INSTDIR\tbb12.dll"
Delete "$INSTDIR\vision-guard-removebg.png"
Delete "$INSTDIR\VisionGuardApp.exe"
Delete "$INSTDIR\translations\qt_ar.qm"
Delete "$INSTDIR\translations\qt_bg.qm"
Delete "$INSTDIR\translations\qt_ca.qm"
Delete "$INSTDIR\translations\qt_cs.qm"
Delete "$INSTDIR\translations\qt_da.qm"
Delete "$INSTDIR\translations\qt_de.qm"
Delete "$INSTDIR\translations\qt_en.qm"
Delete "$INSTDIR\translations\qt_es.qm"
Delete "$INSTDIR\translations\qt_fa.qm"
Delete "$INSTDIR\translations\qt_fi.qm"
Delete "$INSTDIR\translations\qt_fr.qm"
Delete "$INSTDIR\translations\qt_gd.qm"
Delete "$INSTDIR\translations\qt_he.qm"
Delete "$INSTDIR\translations\qt_hr.qm"
Delete "$INSTDIR\translations\qt_hu.qm"
Delete "$INSTDIR\translations\qt_it.qm"
Delete "$INSTDIR\translations\qt_ja.qm"
Delete "$INSTDIR\translations\qt_ka.qm"
Delete "$INSTDIR\translations\qt_ko.qm"
Delete "$INSTDIR\translations\qt_lv.qm"
Delete "$INSTDIR\translations\qt_nl.qm"
Delete "$INSTDIR\translations\qt_nn.qm"
Delete "$INSTDIR\translations\qt_pl.qm"
Delete "$INSTDIR\translations\qt_pt_BR.qm"
Delete "$INSTDIR\translations\qt_ru.qm"
Delete "$INSTDIR\translations\qt_sk.qm"
Delete "$INSTDIR\translations\qt_tr.qm"
Delete "$INSTDIR\translations\qt_uk.qm"
Delete "$INSTDIR\translations\qt_zh_CN.qm"
Delete "$INSTDIR\translations\qt_zh_TW.qm"
Delete "$INSTDIR\tls\qcertonlybackend.dll"
Delete "$INSTDIR\tls\qopensslbackend.dll"
Delete "$INSTDIR\tls\qschannelbackend.dll"
Delete "$INSTDIR\styles\qmodernwindowsstyle.dll"
Delete "$INSTDIR\platforms\qwindows.dll"
Delete "$INSTDIR\omz_models\open-closed-eye-0001\open-closed-eye.onnx"
Delete "$INSTDIR\omz_models\open-closed-eye-0001\FP32\open-closed-eye-0001.bin"
Delete "$INSTDIR\omz_models\open-closed-eye-0001\FP32\open-closed-eye-0001.xml"
Delete "$INSTDIR\omz_models\open-closed-eye-0001\FP16-INT8\open-closed-eye-0001.bin"
Delete "$INSTDIR\omz_models\open-closed-eye-0001\FP16-INT8\open-closed-eye-0001.xml"
Delete "$INSTDIR\omz_models\open-closed-eye-0001\FP16\open-closed-eye-0001.bin"
Delete "$INSTDIR\omz_models\open-closed-eye-0001\FP16\open-closed-eye-0001.xml"
Delete "$INSTDIR\omz_models\head-pose-estimation-adas-0001\FP32\head-pose-estimation-adas-0001.bin"
Delete "$INSTDIR\omz_models\head-pose-estimation-adas-0001\FP32\head-pose-estimation-adas-0001.xml"
Delete "$INSTDIR\omz_models\head-pose-estimation-adas-0001\FP16-INT8\head-pose-estimation-adas-0001.bin"
Delete "$INSTDIR\omz_models\head-pose-estimation-adas-0001\FP16-INT8\head-pose-estimation-adas-0001.xml"
Delete "$INSTDIR\omz_models\head-pose-estimation-adas-0001\FP16\head-pose-estimation-adas-0001.bin"
Delete "$INSTDIR\omz_models\head-pose-estimation-adas-0001\FP16\head-pose-estimation-adas-0001.xml"
Delete "$INSTDIR\omz_models\gaze-estimation-adas-0002\FP32\gaze-estimation-adas-0002.bin"
Delete "$INSTDIR\omz_models\gaze-estimation-adas-0002\FP32\gaze-estimation-adas-0002.xml"
Delete "$INSTDIR\omz_models\gaze-estimation-adas-0002\FP16-INT8\gaze-estimation-adas-0002.bin"
Delete "$INSTDIR\omz_models\gaze-estimation-adas-0002\FP16-INT8\gaze-estimation-adas-0002.xml"
Delete "$INSTDIR\omz_models\gaze-estimation-adas-0002\FP16\gaze-estimation-adas-0002.bin"
Delete "$INSTDIR\omz_models\gaze-estimation-adas-0002\FP16\gaze-estimation-adas-0002.xml"
Delete "$INSTDIR\omz_models\facial-landmarks-98-detection-0001\FP32\facial-landmarks-98-detection-0001.bin"
Delete "$INSTDIR\omz_models\facial-landmarks-98-detection-0001\FP32\facial-landmarks-98-detection-0001.xml"
Delete "$INSTDIR\omz_models\facial-landmarks-98-detection-0001\FP16-INT8\facial-landmarks-98-detection-0001.bin"
Delete "$INSTDIR\omz_models\facial-landmarks-98-detection-0001\FP16-INT8\facial-landmarks-98-detection-0001.xml"
Delete "$INSTDIR\omz_models\facial-landmarks-98-detection-0001\FP16\facial-landmarks-98-detection-0001.bin"
Delete "$INSTDIR\omz_models\facial-landmarks-98-detection-0001\FP16\facial-landmarks-98-detection-0001.xml"
Delete "$INSTDIR\omz_models\facial-landmarks-35-adas-0002\FP32\facial-landmarks-35-adas-0002.bin"
Delete "$INSTDIR\omz_models\facial-landmarks-35-adas-0002\FP32\facial-landmarks-35-adas-0002.xml"
Delete "$INSTDIR\omz_models\facial-landmarks-35-adas-0002\FP16-INT8\facial-landmarks-35-adas-0002.bin"
Delete "$INSTDIR\omz_models\facial-landmarks-35-adas-0002\FP16-INT8\facial-landmarks-35-adas-0002.xml"
Delete "$INSTDIR\omz_models\facial-landmarks-35-adas-0002\FP16\facial-landmarks-35-adas-0002.bin"
Delete "$INSTDIR\omz_models\facial-landmarks-35-adas-0002\FP16\facial-landmarks-35-adas-0002.xml"
Delete "$INSTDIR\omz_models\face-detection-retail-0005\FP32\face-detection-retail-0005.bin"
Delete "$INSTDIR\omz_models\face-detection-retail-0005\FP32\face-detection-retail-0005.xml"
Delete "$INSTDIR\omz_models\face-detection-retail-0005\FP16-INT8\face-detection-retail-0005.bin"
Delete "$INSTDIR\omz_models\face-detection-retail-0005\FP16-INT8\face-detection-retail-0005.xml"
Delete "$INSTDIR\omz_models\face-detection-retail-0005\FP16\face-detection-retail-0005.bin"
Delete "$INSTDIR\omz_models\face-detection-retail-0005\FP16\face-detection-retail-0005.xml"
Delete "$INSTDIR\omz_models\face-detection-retail-0004\FP32\face-detection-retail-0004.bin"
Delete "$INSTDIR\omz_models\face-detection-retail-0004\FP32\face-detection-retail-0004.xml"
Delete "$INSTDIR\omz_models\face-detection-retail-0004\FP16-INT8\face-detection-retail-0004.bin"
Delete "$INSTDIR\omz_models\face-detection-retail-0004\FP16-INT8\face-detection-retail-0004.xml"
Delete "$INSTDIR\omz_models\face-detection-retail-0004\FP16\face-detection-retail-0004.bin"
Delete "$INSTDIR\omz_models\face-detection-retail-0004\FP16\face-detection-retail-0004.xml"
Delete "$INSTDIR\omz_models\face-detection-adas-0001\FP32\face-detection-adas-0001.bin"
Delete "$INSTDIR\omz_models\face-detection-adas-0001\FP32\face-detection-adas-0001.xml"
Delete "$INSTDIR\omz_models\face-detection-adas-0001\FP16-INT8\face-detection-adas-0001.bin"
Delete "$INSTDIR\omz_models\face-detection-adas-0001\FP16-INT8\face-detection-adas-0001.xml"
Delete "$INSTDIR\omz_models\face-detection-adas-0001\FP16\face-detection-adas-0001.bin"
Delete "$INSTDIR\omz_models\face-detection-adas-0001\FP16\face-detection-adas-0001.xml"
Delete "$INSTDIR\networkinformation\qnetworklistmanager.dll"
Delete "$INSTDIR\imageformats\qgif.dll"
Delete "$INSTDIR\imageformats\qicns.dll"
Delete "$INSTDIR\imageformats\qico.dll"
Delete "$INSTDIR\imageformats\qjpeg.dll"
Delete "$INSTDIR\imageformats\qpdf.dll"
Delete "$INSTDIR\imageformats\qsvg.dll"
Delete "$INSTDIR\imageformats\qtga.dll"
Delete "$INSTDIR\imageformats\qtiff.dll"
Delete "$INSTDIR\imageformats\qwbmp.dll"
Delete "$INSTDIR\imageformats\qwebp.dll"
Delete "$INSTDIR\iconengines\qsvgicon.dll"
Delete "$INSTDIR\generic\qtuiotouchplugin.dll"
 
RmDir "$INSTDIR\generic"
RmDir "$INSTDIR\iconengines"
RmDir "$INSTDIR\imageformats"
RmDir "$INSTDIR\networkinformation"
RmDir "$INSTDIR\omz_models\face-detection-adas-0001\FP16"
RmDir "$INSTDIR\omz_models\face-detection-adas-0001\FP16-INT8"
RmDir "$INSTDIR\omz_models\face-detection-adas-0001\FP32"
RmDir "$INSTDIR\omz_models\face-detection-retail-0004\FP16"
RmDir "$INSTDIR\omz_models\face-detection-retail-0004\FP16-INT8"
RmDir "$INSTDIR\omz_models\face-detection-retail-0004\FP32"
RmDir "$INSTDIR\omz_models\face-detection-retail-0005\FP16"
RmDir "$INSTDIR\omz_models\face-detection-retail-0005\FP16-INT8"
RmDir "$INSTDIR\omz_models\face-detection-retail-0005\FP32"
RmDir "$INSTDIR\omz_models\facial-landmarks-35-adas-0002\FP16"
RmDir "$INSTDIR\omz_models\facial-landmarks-35-adas-0002\FP16-INT8"
RmDir "$INSTDIR\omz_models\facial-landmarks-35-adas-0002\FP32"
RmDir "$INSTDIR\omz_models\facial-landmarks-98-detection-0001\FP16"
RmDir "$INSTDIR\omz_models\facial-landmarks-98-detection-0001\FP16-INT8"
RmDir "$INSTDIR\omz_models\facial-landmarks-98-detection-0001\FP32"
RmDir "$INSTDIR\omz_models\gaze-estimation-adas-0002\FP16"
RmDir "$INSTDIR\omz_models\gaze-estimation-adas-0002\FP16-INT8"
RmDir "$INSTDIR\omz_models\gaze-estimation-adas-0002\FP32"
RmDir "$INSTDIR\omz_models\head-pose-estimation-adas-0001\FP16"
RmDir "$INSTDIR\omz_models\head-pose-estimation-adas-0001\FP16-INT8"
RmDir "$INSTDIR\omz_models\head-pose-estimation-adas-0001\FP32"
RmDir "$INSTDIR\omz_models\open-closed-eye-0001\FP16"
RmDir "$INSTDIR\omz_models\open-closed-eye-0001\FP16-INT8"
RmDir "$INSTDIR\omz_models\open-closed-eye-0001\FP32"
RmDir "$INSTDIR\omz_models\open-closed-eye-0001"
RmDir "$INSTDIR\platforms"
RmDir "$INSTDIR\styles"
RmDir "$INSTDIR\tls"
RmDir "$INSTDIR\translations"
 
Delete "$INSTDIR\uninstall.exe"
!ifdef WEB_SITE
Delete "$INSTDIR\${APP_NAME} website.url"
!endif

RmDir "$INSTDIR"

!ifdef REG_START_MENU
!insertmacro MUI_STARTMENU_GETFOLDER "Application" $SM_Folder
Delete "$SMPROGRAMS\$SM_Folder\${APP_NAME}.lnk"
Delete "$SMPROGRAMS\$SM_Folder\Uninstall ${APP_NAME}.lnk"
!ifdef WEB_SITE
Delete "$SMPROGRAMS\$SM_Folder\${APP_NAME} Website.lnk"
!endif
Delete "$DESKTOP\${APP_NAME}.lnk"

RmDir "$SMPROGRAMS\$SM_Folder"
!endif

!ifndef REG_START_MENU
Delete "$SMPROGRAMS\VisionGuard\${APP_NAME}.lnk"
Delete "$SMPROGRAMS\VisionGuard\Uninstall ${APP_NAME}.lnk"
!ifdef WEB_SITE
Delete "$SMPROGRAMS\VisionGuard\${APP_NAME} Website.lnk"
!endif
Delete "$DESKTOP\${APP_NAME}.lnk"

RmDir "$SMPROGRAMS\VisionGuard"
!endif

DeleteRegKey ${REG_ROOT} "${REG_APP_PATH}"
DeleteRegKey ${REG_ROOT} "${UNINSTALL_PATH}"
SectionEnd

######################################################################

