package components

import (
	"pm/device"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/canvas"
	"fyne.io/fyne/v2/container"
)

type StatusWindow struct {
}

func NewStatusWindow(device *device.Device) *fyne.Container {
	statusIcon := &canvas.Image{}
	statusText := &canvas.Text{}

	if device.IsConnected {
		statusIcon = canvas.NewImageFromResource(resourcePerformanceMonitorIco)
		statusText.Text = "Device connected"
	} else {
		statusIcon = canvas.NewImageFromResource(resourcePerformanceMonitorIco)
		statusText.Text = "Device not connected"
	}
	statusIcon.FillMode = canvas.ImageFillCover
	statusIcon.SetMinSize(fyne.NewSize(64, 64))
	statusText.TextSize = 16

	iconTextContainer := container.NewVBox(statusIcon, statusText)
	// iconTextContainer.Resize(fyne.NewSize(800, 720))
	container := container.NewPadded(container.NewCenter(iconTextContainer))
	// container.Resize(fyne.NewSize(800, 720))

	return container
}
