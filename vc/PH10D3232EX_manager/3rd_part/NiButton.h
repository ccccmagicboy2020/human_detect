//==============================================================================
//
//  Title     : NiButton.h
//  Copyright : National Instruments 1999.  All Rights Reserved.
//  Purpose   : Declares the CNiButton class.
//
//==============================================================================
#pragma once


//==============================================================================
//  Includes
//==============================================================================
#include "NiCustomInterface.h"
#include "NiUIInterfaces.h"
#include "NiBindings.h"
#include "NiColor.h"
#include "NiControl.h"
#include "NiInterface.h"
#include "NiComInitialize.h"


//==============================================================================
//  Namespace Directives
//==============================================================================
namespace NI {


//==============================================================================
//  Forward References
//==============================================================================
class CNiFont;
class CNiImage;
class CNiPicture;

// CNiButton encapsulates the interface to the Measurement Studio ActiveX 
// button control, which represents different Boolean displays such as on/off or
// true/false. Typically, you often use buttons to input or output Boolean information
// or initiate an action in your program.
class CNiButton : public CNiControl {
    public:
        //---------------------------------------------------------------------
        // Enumerations
        //---------------------------------------------------------------------
        
        // Button styles.
        enum ButtonStyles {
            // Vertical toggle.
            VToggle                 = 0, 
            // Horizontal toggle.
            HToggle                 = 1, 
            // Square LED.
            SquareLED               = 2, 
            // Square star LED.
            SquareStarLED           = 3, 
            // Round LED.
            RoundLED                = 4, 
            // Round star LED.
            RoundStarLED            = 5, 
            // Round push button.
            RoundPushButton         = 6, 
            // Square push button.
            SquarePushButton        = 7, 
            // 3D vertical slide.
            V3dSlide                = 8, 
            // 3D horizontal slide.
            H3dSlide                = 9, 
            // Vertical slide.
            VSlide                  = 10, 
            // Horizontal slide.
            HSlide                  = 11, 
            // On/off toggle.
            ToggleOnOff             = 12, 
            // Command button.
            CommandOk               = 13, 
            // Custom bitmap button.
            Bitmap                  = 14, 
            // 3D Vertical toggle.
            VToggle3D               = 15, 
            // 3D Horizontal toggle.
            HToggle3D               = 16, 
            // 3D Square LED.
            SquareLED3D             = 17, 
            // 3D Round LED.
            RoundLED3D              = 18, 
            // 3D Push Button.
            PushButton3D            = 19, 
            // 3D Vertical rocker.
            VRocker3D               = 20, 
            // 3D Horizontal rocker.
            HRocker3D               = 21, 
            // 3D Vertical slide switch.
            VSlideSwitch3D          = 22, 
            // 3D Horizontal slide switch.
            HSlideSwitch3D          = 23, 
            // 3D On/off toggle.
            ToggleOnOff3D           = 24, 
            // 3D Command button.
            CommandOk3D             = 25, 
            // 3D Custom bitmap button.
            Bitmap3D                = 26          
        };

        // Button modes.
        enum ButtonModes {
            // When you click the mouse on
            // the CNiButton, the value 
            // changes and remains until you
            // click on the CNiButton again.
            ModeSwitchWhenPressed   = 1, 
            // When you click the mouse on 
            // the CNiButton, the value 
            // changes and remains until you 
            // release button.
            ModeSwitchUntilReleased = 2, 
            // The CNiButton does not 
            // respond to user input. In 
            // this mode, you are limited 
            // to changing the value of the 
            // CNiButton programmatically.
            ModeIndicator           = 3     
        };

        // Keyboard modes.
        enum KeyboardModes {
            // The control ignores keystrokes.
            KeyboardNone            = 0, 
            // The control processes keystrokes.
            KeyboardHandled         = 1     
        };

        // Show focus modes.
        enum ShowFocusModes {
            // Does not indicate graphically if the 
            // control has the focus.
            ShowFocusNone           = 0, 
            // Indicates graphically if the control has 
            // the focus.
            ShowFocusControl        = 1
        };

        // Mouse buttons.
        enum MouseButtons {
            // The user pressed the left mouse button.
            LeftButton              = 1, 
            // The user pressed the right mouse button.
            RightButton             = 2, 
            // The user pressed the middle mouse button.
            MiddleButton            = 4     
        };

        // Shift keys.
        enum ShiftKeys {
            // The user pressed the SHIFT key.
            ShiftKey                = 1, 
            // The user pressed the CTRL key.
            CtrlKey                 = 2, 
            // The user pressed the ALT key.
            AltKey                  = 4     
        };

        //---------------------------------------------------------------------
        //  Functions
        //---------------------------------------------------------------------

        // Sets many properties of the control to represent the new style 
        // specified.
        void SetBuiltinStyle (
            // The button style to be used. The following list includes valid 
            // button styles.
            // 
            // * CNiButton::VToggle - vertical toggle.
            // * CNiButton::HToggle - horizontal toggle.
            // * CNiButton::SquareLED - square LED.
            // * CNiButton::SquareStarLED - square star LED.
            // * CNiButton::RoundLED - round LED.
            // * CNiButton::RoundStarLED - round star LED.
            // * CNiButton::RoundPushButton - round push button.
            // * CNiButton::SquarePushButton - square push button.
            // * CNiButton::V3dSlide - 3D vertical slide.
            // * CNiButton::H3dSlide - 3D horizontal slide.
            // * CNiButton::VSlide - vertical slide.
            // * CNiButton::HSlide - horizontal slide.
            // * CNiButton::ToggleOnOff - on/off toggle.
            // * CNiButton::CommandOk - command button.
            // * CNiButton::Bitmap - custom bitmap button.
            // * CNiButton::VToggle3D - 3D vertical toggle.
            // * CNiButton::HToggle3D - 3D horizontal toggle.
            // * CNiButton::SquareLED3D - 3D square LED.
            // * CNiButton::RoundLED3D - 3D round LED.
            // * CNiButton::PushButton3D - 3D push Button.
            // * CNiButton::VRocker3D - 3D vertical rocker.
            // * CNiButton::HRocker3D - 3D horizontal rocker.
            // * CNiButton::VSlideSwitch3D - 3D vertical slide switch.
            // * CNiButton::HSlideSwitch3D - 3D horizontal slide switch.
            // * CNiButton::ToggleOnOff3D - 3D on/off toggle.
            // * CNiButton::CommandOk3D - 3D command button.
            // * CNiButton::Bitmap3D - 3D custom bitmap button.
            ButtonStyles style);

        // Displays the About Box for the control.
        void AboutBox ();

        // Imports a previously exported style.
        void ImportStyle (
            // The name of the style file to load.
            LPCTSTR fileName);

        // Exports the style of the control to a file.
        void ExportStyle (
            // The name of the style file to save.
            LPCTSTR fileName);

        // Gets the image object corresponding to the On state of 
        // the CNiButton control that is associated with the specified part of 
        // the CNiButton control.
        CNiImage OnImages (
            // Specifies the index of the image object to return. The list 
            // below includes valid image indices.
            //
            // * CNiButton::1 - caption
            // * CNiButton::2 - background
            // * CNiButton::3 - on Text
            // * CNiButton::4 - off Text
            // * CNiButton::5 - image
            long imageIndex);

        // Gets the image object corresponding to the On state of 
        // the CNiButton control that is associated with the specified part of 
        // the CNiButton control.
        CNiImage OnImages (
            // Specifies the name of the image object to return. The list below 
            // includes valid image names.
            //
            // * CNiButton::Caption
            // * CNiButton::Background
            // * CNiButton::On Text
            // * CNiButton::Off Text
            // * CNiButton::Image
            const CString& imageName);

        // Gets the image object corresponding to the Off state of 
        // the CNiButton control that is associated with the specified part of 
        // the CNiButton control.
        CNiImage OffImages (
            // Specifies the index of the image object to return. The list 
            // below includes valid image indices.
            //
            // * CNiButton::1 - caption
            // * CNiButton::2 - background
            // * CNiButton::3 - on Text
            // * CNiButton::4 - off Text
            // * CNiButton::5 - image
            long imageIndex);

        // Gets the image object corresponding to the Off state of 
        // the CNiButton control that is associated with the specified part of 
        // the CNiButton control.
        CNiImage OffImages (
            // Specifies the name of the image object to return. The list
            // below includes valid image names.
            //
            // * CNiButton::Caption
            // * CNiButton::Background
            // * CNiButton::On Text
            // * CNiButton::Off Text
            // * CNiButton::Image
             const CString& imageName);

        // Returns an image of the entire control.
        CNiPicture ControlImage ();

        // Forces the control to redraw.
        void Refresh ();

        // Accessor function for BackColor property.
        CNiColor GetBackColor();

        // Accessor function for Bindings property.
        CNiBindings GetBindings();

        // Accessor function for Caption property.
        CString GetCaption();

        // Accessor function for CaptionColor property.
        CNiColor GetCaptionColor();

        // Accessor function for Enabled property.
        bool GetEnabled();

        // Accessor function for Font property.
        CNiFont GetFont();

        // Accessor function for ImmediateUpdates property.
        bool GetImmediateUpdates();

        // Accessor function for KeyboardMode property.
        KeyboardModes GetKeyboardMode();

        // Accessor function for Mode property.
        ButtonModes GetMode();

        // Accessor function for OffColor property.
        CNiColor GetOffColor();

        // Accessor function for OffText property.
        CString GetOffText();

        // Accessor function for OffTextColor property.
        CNiColor GetOffTextColor();

        // Accessor function for OnColor property.
        CNiColor GetOnColor();

        // Accessor function for OnText property.
        CString GetOnText();

        // Accessor function for OnTextColor property.
        CNiColor GetOnTextColor();

        // Accessor function for ReadyState property.
        long GetReadyState();

        // Accessor function for ShowFocusMode property.
        ShowFocusModes GetShowFocusMode();

        // Accessor function for Value property.
        bool GetValue();

        // Accessor function for Windowless property.
        bool GetWindowless();

        // Accessor function for BackColor property.
        void SetBackColor(const CNiColor& backColor);

        // Accessor function for Caption property.
        void SetCaption(const CString& caption);

        // Accessor function for CaptionColor property.
        void SetCaptionColor(const CNiColor& captionColor);

        // Accessor function for Enabled property.
        void SetEnabled(bool enabled);

        // Accessor function for Font property.
        void SetFont(const CNiFont& font);

        // Accessor function for ImmediateUpdates property.
        void SetImmediateUpdates(bool immediateUpdates);

        // Accessor function for KeyboardMode property.
        void SetKeyboardMode(KeyboardModes keyboardMode);

        // Accessor function for Mode property.
        void SetMode(ButtonModes buttonModes);

        // Accessor function for OffColor property.
        void SetOffColor(const CNiColor& offColor);

        // Accessor function for OffText property.
        void SetOffText(const CString& offText);

        // Accessor function for OffTextColor property.
        void SetOffTextColor(const CNiColor& offTextColor);

        // Accessor function for OnColor property.
        void SetOnColor(const CNiColor& onColor);

        // Accessor function for OnText property.
        void SetOnText(const CString& onText);

        // Accessor function for OnTextColor property.
        void SetOnTextColor(const CNiColor& onTextColor);

        // Accessor function for ShowFocusMode property.
        void SetShowFocusMode(ShowFocusModes showFocusMode);

        // Accessor function for Value property.
        void SetValue(bool value);

        // Accessor function for Windowless property.
        void SetWindowless(bool windowless);

        //---------------------------------------------------------------------
        //  Properties
        //---------------------------------------------------------------------

        // Specifies the background color of the button.
        __declspec(property(get=GetBackColor, put=SetBackColor)) CNiColor BackColor;

        // Gets a collection of binding objects associated with the control.
        __declspec(property(get=GetBindings)) CNiBindings Bindings;

        // Specifies the text that appears in the button.
        __declspec(property(get=GetCaption, put=SetCaption)) CString Caption;

        // Specifies the color of the caption for the button.
        __declspec(property(get=GetCaptionColor, put=SetCaptionColor)) CNiColor CaptionColor;

        // Specifies if the control responds to user input.
        __declspec(property(get=GetEnabled, put=SetEnabled)) bool Enabled;

        // Specifies the font for the caption and the two text fields in the 
        // button.
        __declspec(property(get=GetFont, put=SetFont)) CNiFont Font;

        // Specifies if the control draws new data as soon as it is 
        // available or if the form refreshes the control when it draws 
        // other controls.
        __declspec(property(get=GetImmediateUpdates, put=SetImmediateUpdates)) bool ImmediateUpdates;

        // Specifies how the control handles keyboard input from the user.
        // This function returns one of the values listed below.
        // * CNiButton::KeyboardNone - the control ignores keystrokes.
        // * CNiButton::KeyboardHandled - the control processes keystrokes.
        __declspec(property(get=GetKeyboardMode, put=SetKeyboardMode)) KeyboardModes KeyboardMode;

        // Specifies how the button responds to user input. The list below includes
        // valid values.
        // * CNiButton::ModeSwitchWhenPressed - when you click the mouse on the 
        // button, the value changes and remains until you click on the
        // button again.
        // * CNiButton::ModeSwitchUntilReleased - when you click the mouse on the 
        // button, the value of the button changes.  When you release the
        // mouse, the value of the button changes back to its original state.
        // * CNiButton::ModeIndicator - the button does not respond to user input.
        // In this mode, you are limited to changing the value programmatically.
        __declspec(property(get=GetMode, put=SetMode)) ButtonModes Mode;

        // Specifies the color of the button in the off state.
        __declspec(property(get=GetOffColor, put=SetOffColor)) CNiColor OffColor;

        // Specifies the text for the button in the off state.
        __declspec(property(get=GetOffText, put=SetOffText)) CString OffText;

        // Specifies the color of the text for the button in the off state.
        __declspec(property(get=GetOffTextColor, put=SetOffTextColor)) CNiColor OffTextColor;

        // Specifies the color of the button in the on state.
        __declspec(property(get=GetOnColor, put=SetOnColor)) CNiColor OnColor;

        // Specifies the text for the button in the on state.
        __declspec(property(get=GetOnText, put=SetOnText)) CString OnText;

        // Specifies the color of the text for the button in the on state.
        __declspec(property(get=GetOnTextColor, put=SetOnTextColor)) CNiColor OnTextColor;

        // Returns the ready state of the control. This property is a Boolean that
        // indicates if the control is available for interaction before all of its
        // data has been loaded.
        __declspec(property(get=GetReadyState)) long ReadyState;

        // Specifies how the control indicates it has the focus.
        //
        // * CNiButton::ShowFocusNone - does not indicate graphically if the 
        // control has the focus.
        //
        // * CNiButton::ShowFocusControl - indicates graphically if the control
        // has the focus.
        __declspec(property(get=GetShowFocusMode, put=SetShowFocusMode)) ShowFocusModes ShowFocusMode;

        // Specifies the current value of the button.
        __declspec(property(get=GetValue, put=SetValue)) bool Value;

        // Specifies if the control has a window.
        __declspec(property(get=GetWindowless, put=SetWindowless)) bool Windowless;

    protected:
        DECLARE_DYNCREATE(CNiButton)
        NI_DECLARE_CONTROL_CI(_DCWButton_CI, CNiButton);

    private:
        //---------------------------------------------------------------------
        //  Undefined Canonicals
        //---------------------------------------------------------------------

        // Copy constructor.
        CNiButton (const CNiButton&);

        // Assignment operator.
        CNiButton& operator = (const CNiButton&);
};


}  // NI namespace
