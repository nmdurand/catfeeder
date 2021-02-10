import Marionette from 'backbone.marionette'
import template from 'templates/schedule'
import itemTemplate from 'templates/schedule/item'

MAX_SLOTS = 6

class ScheduleItemView extends Marionette.View
	template: itemTemplate
	className: 'list-group-item scheduleItem'

	triggers:
		'click .delete': 'delete'

	initialize: ->
		# console.log 'Initializing Schedule Item View'


export default class ScheduleView extends Marionette.CollectionView
	template: template
	className: 'schedule'

	childView: ScheduleItemView
	childViewContainer: '.scheduleContainer'

	events:
		'click #addBtn': 'addItem'
	childViewEvents:
		'delete': 'handleDeleteChild'

	initialize: ->
		console.log 'Initializing Schedule View'

		@collection = new Backbone.Collection
		@collection.viewComparator = 'time'

	templateContext: ->
		slotsAvailable: => @collection.length < MAX_SLOTS

	addItem: ->
		unless @collection.length >=  MAX_SLOTS
			console.log 'Adding item to schedule'
			@collection.add new Backbone.Model
				time: '12:00'
				qty: 1

	handleDeleteChild: (cv)->
		console.log 'Removing child from collection'
		@collection.remove cv.model
