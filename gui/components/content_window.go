package components

import (
	"pm/device"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/container"
)

type ContentWindow struct {
	Device    *device.Device
	Container *fyne.Container
}

func NewContentWindow(device *device.Device) *ContentWindow {
	cw := &ContentWindow{}
	cw.Device = device
	cw.Container = cw.createContainer()
	cw.ShowComponent(0)

	return cw
}

func (cw *ContentWindow) createContainer() *fyne.Container {
	statusWindow := NewStatusWindow(cw.Device)
	customizationWindow := NewCustomizationWindow(cw.Device)

	container := container.NewStack(statusWindow.Container, customizationWindow.Container)

	return container
}

func (cw *ContentWindow) ShowComponent(id int) {
	for _, obj := range cw.Container.Objects {
		obj.Hide()
	}
	cw.Container.Objects[id].Show()
}
