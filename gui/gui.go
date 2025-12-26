package gui

import (
	"pm/device"
	"pm/gui/components"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/app"
	"fyne.io/fyne/v2/container"
)

func Run() {
	app := app.New()
	device := device.Device{
		IsConnected: true,
	}
	window := app.NewWindow("MSTL Performance Monitor")
	// colorScheme := colorschemes.GetColorScheme(colorschemes.ColorSchemeLight)

	// topMenu := components.TopMenu.CreateComponent(components.TopMenu{}, &device, &colorScheme)
	windowContent := createWindowContent(&device)

	window.Resize(fyne.NewSize(1280, 720))
	window.SetContent(windowContent)
	window.ShowAndRun()
}

func createWindowContent(device *device.Device) *fyne.Container {
	navMenu := components.NewNavMenuContainer()
	contentWindow := components.NewContentWindow(device)

	split := container.NewHSplit(navMenu, contentWindow.Container)
	split.SetOffset(0.25)
	mainContainer := container.NewPadded(split)

	return mainContainer
}
