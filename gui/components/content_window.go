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
	contentWindow := &ContentWindow{}
	contentWindow.Device = device
	contentWindow.Container = contentWindow.createContainer()
	
	return contentWindow
}

func (contentWindow *ContentWindow) createContainer() *fyne.Container {
	statusWindow := NewStatusWindow(contentWindow.Device)

	container := container.NewStack(statusWindow.Container)

	return container
}

func (contentWindow *ContentWindow) createItems() []*fyne.Container {
	statusWindow := NewStatusWindow(contentWindow.Device)

	return []*fyne.Container{statusWindow.Container}
}

func (contentWindow *ContentWindow) ShowComponent(id int) {
	for _, obj := range contentWindow.Container.Objects {
		obj.Hide()
	}
	contentWindow.Container.Objects[id].Show()
}
