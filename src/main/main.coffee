path = require 'path'
fs = require('fs').promises
url = require 'url'
electron = require 'electron'
{ app, BrowserWindow, ipcMain } = electron
_ = require 'lodash'


console.log 'Running electron.'


# Keep a global reference of the window object, if you don't, the window will
# be closed automatically when the JavaScript object is garbage collected.
mainWindow = null

if process.env.NODE_ENV is 'development'
	# Initialize electron-reload
	require('electron-reload') __dirname,
		electron: path.join(process.cwd(), 'node_modules', '.bin', 'electron')

# Quit when all windows are closed.
app.on 'window-all-closed', ->
	# On OS X it is common for applications and their menu bar
	# to stay active until the user quits explicitly with Cmd + Q
	if process.platform isnt 'darwin'
		app.quit()

app.on 'ready', ->
	mainWindow = new BrowserWindow
		resizable: true
		fullscreenable: false
		width: 768
		height: 700
		minWidth: 768
		webPreferences:
			nodeIntegration: true
			contextIsolation: false

	mainWindow.loadURL url.format
		protocol: 'file:'
		pathname: path.join __dirname, './renderer/index.html'
		slashes: true

	if process.env.NODE_ENV is 'development'
		# Open the DevTools.
		mainWindow.webContents.openDevTools
			mode: 'undocked'

	mainWindow.on 'closed', ->
		win = null
		process.exit(0)

	########### Communication with renderer process
	# ipcMain.on event, handler
