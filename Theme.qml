pragma Singleton
import QtQuick

// * Single source of truth for the app's look — colors, spacing, radii, timings.
// * Every screen reads from here so flipping `dark` re-themes the whole app at once.
QtObject {
    id: theme

    property bool dark: true

    // Dark palette
    readonly property color darkBg: "#0B0D12"
    readonly property color darkSurface: "#12141B"
    readonly property color darkCard: "#171A22"
    readonly property color darkCardAlt: "#1D2029"
    readonly property color darkBorder: "#262A35"
    readonly property color darkText: "#F4F5F7"
    readonly property color darkMuted: "#8B92A5"

    // Light palette
    readonly property color lightBg: "#F3F4F7"
    readonly property color lightSurface: "#FFFFFF"
    readonly property color lightCard: "#FFFFFF"
    readonly property color lightCardAlt: "#F7F8FA"
    readonly property color lightBorder: "#E3E5EA"
    readonly property color lightText: "#14161C"
    readonly property color lightMuted: "#666E80"

    // Active palette (reactively swaps when `dark` flips — pair with
    // `Behavior on color` at the point of use for a smooth cross-fade)
    property color background: dark ? darkBg : lightBg
    property color surface: dark ? darkSurface : lightSurface
    property color card: dark ? darkCard : lightCard
    property color cardAlt: dark ? darkCardAlt : lightCardAlt
    property color border: dark ? darkBorder : lightBorder
    property color textPrimary: dark ? darkText : lightText
    property color textMuted: dark ? darkMuted : lightMuted

    readonly property color accent: "#6366F1"
    readonly property color accentHover: "#7B7EF5"
    readonly property color accentPressed: "#5254CC"
    readonly property color success: "#22C55E"
    readonly property color danger: "#EF4444"
    readonly property color dangerHover: "#F65F5F"
    readonly property color warning: "#F59E0B"

    readonly property int radiusSmall: 8
    readonly property int radiusMedium: 12
    readonly property int radiusLarge: 18
    readonly property int radiusPill: 999

    readonly property int spacingTiny: 4
    readonly property int spacingSmall: 8
    readonly property int spacingMedium: 16
    readonly property int spacingLarge: 24
    readonly property int spacingXLarge: 32

    readonly property int animFast: 120
    readonly property int animMedium: 220
    readonly property int animSlow: 380

    readonly property int fontTiny: 11
    readonly property int fontSmall: 12
    readonly property int fontBody: 14
    readonly property int fontMedium: 16
    readonly property int fontLarge: 20
    readonly property int fontXLarge: 26
}
