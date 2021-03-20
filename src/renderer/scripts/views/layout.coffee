import Marionette from 'backbone.marionette'
import template from 'templates/layout'
import ScheduleView from 'views/schedule'

export default class LayoutView extends Marionette.View
	template: template
	className: 'layout'

	regions:
		editorRegion: '#editorRegion'

	initialize: ->
		console.log 'Initializing Layout View'

		@showChildView 'editorRegion', new ScheduleView
