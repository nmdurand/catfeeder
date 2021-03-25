import {Application} from 'backbone.marionette'
import LayoutView from 'views/layout'
import requestUtils from 'lib/request'


export default class App extends Application
	initialize: ->
		console.debug 'Initializing app'

	onStart: ->
		console.debug 'Starting App'
		await requestUtils.connectServer()
		@showView new LayoutView
