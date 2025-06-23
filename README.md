# QCyberCafe
QT 5.12 based CyberCafe Kiosk Software

WIP test at own risk

✅ NEW FEATURES:
Hides the Dock and menu bar
Blocks Force Quit dialog (Cmd+Option+Esc)
Disables Cmd+Q, Cmd+Tab, etc.
Uses accessibility APIs to restrict system-level key combos
Prevents mouse movement and hides cursor
Remains always on top
⚠️ macOS sandbox protections (like SIP) prevent complete locking from normal apps. You'll need:
Accessibility permissions (manually grant or via .plist)
Running the app as root or with launch agent/helper
Possibly disabling SIP (not recommended in production)




✅ Extra Security Tips

Technique	How
Run at startup	Use ~/Library/LaunchAgents plist
Prevent Cmd+Opt+Esc (Force Quit)	Needs AXUIElement accessibility API
Lock down desktop	Remove Finder, Dock temporarily
Prevent restart/shutdown	Hook loginwindow with helper



🧭 Instructions to Use It

🗂️ 1. Place your app binary
Example location:

/Applications/CyberLocker/locker
Make sure it's executable:

chmod +x /Applications/CyberLocker/locker
📂 2. Save plist here
~/Library/LaunchAgents/com.cybercafe.locker.plist
If it doesn't exist, create the folder:

mkdir -p ~/Library/LaunchAgents
▶️ 3. Load the agent
launchctl load ~/Library/LaunchAgents/com.cybercafe.locker.plist
To auto-start after reboot, nothing else needed. It's persistent.

✅ Optional: To Unload or Disable

launchctl unload ~/Library/LaunchAgents/com.cybercafe.locker.plist