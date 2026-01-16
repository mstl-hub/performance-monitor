package components

import (
	"pm/device"
	"pm/gui/profile"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/container"
)

type ContentWindow struct {
	Device    *device.Device
	Profiles  *profile.Profiles
	Container *fyne.Container
}

func NewContentWindow(device *device.Device, profiles *profile.Profiles) *ContentWindow {
	cw := &ContentWindow{}
	cw.Device = device
	cw.Profiles = profiles
	cw.Container = cw.createContainer()
	cw.ShowComponent(0)

	return cw
}

func (cw *ContentWindow) createContainer() *fyne.Container {
	statusWindow := NewStatusWindow(cw.Device)
	customizationWindow := NewCustomizationWindow(cw.Device, cw.Profiles)

	container := container.NewStack(statusWindow.Container, customizationWindow.Container)

	return container
}

func (cw *ContentWindow) ShowComponent(id int) {
	for _, obj := range cw.Container.Objects {
		obj.Hide()
	}
	cw.Container.Objects[id].Show()
}
