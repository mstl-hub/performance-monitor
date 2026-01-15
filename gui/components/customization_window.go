package components

import (
	"pm/device"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/container"
)

type CustomizationWindow struct {
	Device          *device.Device
	Container       *fyne.Container
	CustomizatonBar *CustomizationBar
	PreviewWindow   *PreviewWindow
}

func NewCustomizationWindow(device *device.Device) *CustomizationWindow {
	cw := &CustomizationWindow{}
	cw.Device = device
	cw.CustomizatonBar = NewCustomizationBar(cw.Device)
	cw.PreviewWindow = NewPreviewWindow(cw.Device)

	container := container.NewVBox(cw.CustomizatonBar.Container, cw.PreviewWindow.Container)
	cw.Container = container

	return cw
}
