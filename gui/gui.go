package gui

import (
	"embed"
	"pm/device"
	"pm/gui/components"
	"pm/gui/profile"
	"pm/gui/themes"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/app"
	"fyne.io/fyne/v2/container"
	"fyne.io/fyne/v2/lang"
)

//go:embed translation
var translations embed.FS

func Run() {
	app := app.New()

	lang.AddTranslationsFS(translations, "translation")

	app.Settings().SetTheme(themes.NewAppTheme())

	device := &device.Device{
		IsConnected: true,
	}

	profiles := &profile.Profiles{}

	window := app.NewWindow("MSTL Performance Monitor")
	// colorScheme := colorschemes.GetColorScheme(colorschemes.ColorSchemeLight)

	// topMenu := components.TopMenu.CreateComponent(components.TopMenu{}, &device, &colorScheme)
	windowContent := createWindowContent(device, profiles)

	window.Resize(fyne.NewSize(1280, 720))
	window.SetContent(windowContent)
	window.ShowAndRun()
}

func createWindowContent(device *device.Device, profiles *profile.Profiles) *fyne.Container {
	contentWindow := components.NewContentWindow(device, profiles)
	navMenu := components.NewNavMenu(contentWindow)
	navMenu.ContentWindow = contentWindow

	split := container.NewHSplit(navMenu.Container, contentWindow.Container)
	split.SetOffset(0.25)
	mainContainer := container.NewPadded(split)

	return mainContainer
}
