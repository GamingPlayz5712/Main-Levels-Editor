# Fangame Creator 🛠️

A Geometry Dash mod built with the **Geode SDK** that allows players to easily manage, edit, and create custom fangames and localized level variations right inside the game client on both Android and Windows.

## Features

- **Main Level Editor UI**: Access a custom settings panel directly from any official level's info screen or pause menu.
- **Automatic Folder Management**: Instantly creates configuration directories at startup under `geode/config/com.delish.FangameCreator/`.
- **Integrated FilePicker**: Seamlessly browse your device storage to import customized `.txt` level structures directly into the mod's target folder.
- **Custom Achievement Storage**: Includes a pre-configured `achievements.json` template ready to read and handle custom gaming milestones.

## How to make and import a custom level (.txt)

To share or craft a level file compatible with this mod, follow these steps:
1. Open the **Geode** mod loader in Geometry Dash.
2. Download and install the **GDShare** mod.
3. Export your desired layout/creation as a `.gmd` file.
4. Open a file manager (like **ZArchiver** on Android).
5. Open the exported `.gmd` file as plain text.
6. Copy all the encoded text from the file.
7. Create a new `.txt` file, paste the copied text inside, and save it.
8. Open Geometry Dash, click the **Gear Icon** inside Fangame Creator, and tap **Import Level .txt**.
9. Select your saved file using the native file picker.

## Directory Structure

Once active, the mod tracks files inside the following path:
`Android/media/com.geode.launcher/game/geode/config/com.delish.FangameCreator/`

- `/levels/`: Destination folder where imported level files are cloned.
- `achievements.json`: Registry for current and upcoming achievements customization.
- `last_edited.txt`: Stores your latest configured metadata (Name, Stars, and Difficulty).

## Developer Setup (Termux / Linux)

If you wish to compile this project manually on a Linux shell or Android Termux environment:
1. Ensure you have the **Android NDK (r26b)** tailored for your architecture.
2. Install the **Geode CLI** via Cargo.
3. Run the following deployment script:
```bash
mkdir build && cd build
cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

## Credits

- Developed by **Delish**.
- Powered by the [Geode SDK](https://geode-sdk.org).
