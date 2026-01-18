package components

import (
	"pm/device"
	"pm/gui/profile"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/container"
)

type CustomizationWindow struct {
	Device          *device.Device
	Profiles        *profile.Profiles
	Container       *fyne.Container
	PreviewWindow   *PreviewWindow
	CustomizatonBar *CustomizationBar
}

func NewCustomizationWindow(device *device.Device, profiles *profile.Profiles) *CustomizationWindow {
	cw := &CustomizationWindow{}
	cw.Device = device
	cw.Profiles = profiles
	cw.PreviewWindow = NewPreviewWindow(cw.Device)
	cw.CustomizatonBar = NewCustomizationBar(cw.Device, cw.Profiles, cw.PreviewWindow)

	container := container.NewVBox(cw.CustomizatonBar.Container, cw.PreviewWindow.Container)
	cw.Container = container

	return cw
}
