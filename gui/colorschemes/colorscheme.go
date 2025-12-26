package colorschemes

import "image/color"

type ColorScheme struct {
	background color.RGBA
	text       color.RGBA
	border     color.RGBA
	prime      color.RGBA
	second     color.RGBA
}

func GetColorScheme(colorSchemeType ColorSchemeType) ColorScheme {
	switch colorSchemeType {
	case ColorSchemeLight:
		return getColorSchemeLight()
	default:
		return getColorSchemeDark()
	}
}

func getColorSchemeLight() ColorScheme {
	return ColorScheme{
		background: color.RGBA{240, 240, 240, 255},
		text:       color.RGBA{0, 0, 0, 255},
		border:     color.RGBA{184, 184, 184, 255},
		prime:      color.RGBA{67, 74, 165, 255},
		second:     color.RGBA{89, 64, 95, 255},
	}
}

func getColorSchemeDark() ColorScheme {
	return ColorScheme{
		background: color.RGBA{32, 32, 32, 255},
		text:       color.RGBA{224, 224, 224, 255},
		border:     color.RGBA{49, 49, 49, 255},
		prime:      color.RGBA{67, 74, 165, 255},
		second:     color.RGBA{89, 64, 95, 255},
	}
}

func UpdateColorScheme(colorScheme *ColorScheme, colorSchemeType ColorSchemeType) {
	newColorScheme := GetColorScheme(colorSchemeType)
	colorScheme.background = newColorScheme.background
	colorScheme.text = newColorScheme.text
	colorScheme.border = newColorScheme.border
	colorScheme.prime = newColorScheme.prime
	colorScheme.second = newColorScheme.second
}
