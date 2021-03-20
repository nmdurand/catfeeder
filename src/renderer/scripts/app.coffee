import {Application} from 'backbone.marionette'
import LayoutView from 'views/layout'


export default class App extends Application
	initialize: ->
		console.debug 'Initializing app'

	onStart: ->
		console.debug 'Starting App'
		@showView new LayoutView
