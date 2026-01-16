package components

import (
	"pm/device"
	"pm/gui/profile"
	"pm/gui/subcomponents"
	"pm/gui/themes"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/container"
	"fyne.io/fyne/v2/dialog"
	"fyne.io/fyne/v2/lang"
	"fyne.io/fyne/v2/theme"
	"fyne.io/fyne/v2/widget"
)

type CustomizationBar struct {
	Device        *device.Device
	Container     *fyne.Container
	PreviewWindow *PreviewWindow
	Profiles      *profile.Profiles
}

func NewCustomizationBar(device *device.Device, profiles *profile.Profiles, previewWindow *PreviewWindow) *CustomizationBar {
	cb := &CustomizationBar{}
	cb.Device = device
	cb.Profiles = profiles
	cb.PreviewWindow = previewWindow

	container := container.NewBorder(nil, nil, nil, nil, nil)
	cb.Container = container

	return cb
}

func (cb *CustomizationBar) createContainer() *fyne.Container {
	deviceThemesIcon := &widget.Icon{}
	deviceThemesSelect := &widget.Select{}
	loadImageButton := &widget.Button{}
	saveChangesButton := &subcomponents.TappableIcon{}

	deviceThemesIcon.SetResource(theme.Current().Icon(themes.IconPerformanceMonitor))

	deviceThemesSelect.SetOptions(cb.Device.Themes)
	deviceThemesSelect.OnChanged = func(s string) {
		index := device.DeviceTheme(deviceThemesSelect.SelectedIndex())
		cb.PreviewWindow.SetDeviceTheme(index)
		cb.Profiles.Selected().DeviceTheme = index
	}

	loadImageButton.SetText(lang.L("Image"))
	loadImageButton.Alignment = widget.ButtonAlignCenter
	loadImageButton.Icon = theme.Current().Icon(themes.IconPerformanceMonitor)
	loadImageButton.IconPlacement = widget.ButtonIconLeadingText
	loadImageButton.OnTapped = func() {
		dialog.ShowFileOpen(func(reader fyne.URIReadCloser, err error) {
			if err != nil {
				return
			}
			var image []byte
			_, er := reader.Read(image)
			if er != nil {
				return
			}
			cb.PreviewWindow.SetImage(image)
			cb.Profiles.Selected().Image = image
		}, fyne.CurrentApp().Driver().AllWindows()[0])
	}

	saveChangesButton.Icon.SetResource(theme.Current().Icon(themes.IconPerformanceMonitor))
	saveChangesButton.OnTapped = func() {
		cb.Profiles.SaveSelected()
	}

	leftContainer := container.NewPadded(container.NewHBox(deviceThemesIcon, deviceThemesSelect, loadImageButton))
	rightContainer := container.NewPadded(container.NewHBox(saveChangesButton))

	container := container.NewBorder(nil, nil, leftContainer, rightContainer, nil)

	return container
}
