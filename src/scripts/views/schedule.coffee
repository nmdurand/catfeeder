import Marionette from 'backbone.marionette'
import Toastr from 'toastr'
import template from 'templates/schedule'
import itemTemplate from 'templates/schedule/item'
import _ from 'lodash'

import RequestUtils from 'lib/request'

MAX_SLOTS = 6
DISABLED_CLASS = 'disabledItem'

asDoubleDigit = (t)->
	if t < 10
		t = "0" + t
	"#{t}"

class ScheduleItemView extends Marionette.View
	template: itemTemplate
	className: 'list-group-item scheduleItem'

	triggers:
		'click .delete': 'delete'

	events:
		'click .disable': 'handleDisable'

	bindings:
		'.time':
			observe: ['h','m']
			onGet: (values)->
				asDoubleDigit(values[0]) + ":" + asDoubleDigit(values[1])
			onSet: (value)->
				values = value.split ':'
				_.map values, (val)-> parseInt val, 10
		'.quantity':
			observe: 'q'
			onSet: (value)->
				parseInt value, 10

	initialize: ->
		# console.log 'Initializing Schedule Item View'

	onRender: ->
		@stickit()
		@updateDisabledState()

	handleDisable: ->
		if @model.get('s') is 0
			@model.set 's', 1
		else
			@model.set 's', 0
		@updateDisabledState()

	updateDisabledState: ->
		if @model.get('s') is 0
			@$el.addClass DISABLED_CLASS
		else
			@$el.removeClass DISABLED_CLASS

export default class ScheduleView extends Marionette.CollectionView
	template: template
	className: 'schedule'

	childView: ScheduleItemView
	childViewContainer: '.scheduleContainer'

	ui:
		'addBtn': '#addBtn'
		'saveBtn': '#saveBtn'

	events:
		'click @ui.addBtn': 'addItem'
		'click @ui.saveBtn': 'setArduinoSchedule'

	childViewEvents:
		'delete': 'handleDeleteChild'

	initialize: ->
		console.log 'Initializing Schedule View'

		@collection = new Backbone.Collection

		@collection.on 'add remove', => @updateAddBtnVisibility()

		@updateScheduleData()

	onRender: ->
		@updateAddBtnVisibility()

	updateScheduleData: (scheduleDetails)=>
		unless scheduleDetails?
			try
				scheduleDetails = await RequestUtils.getSchedule()
			catch err
				console.error 'Error fetching schedule:', err
				Toastr.error 'Error fetching schedule.'

		@collection.set scheduleDetails

	updateAddBtnVisibility: ->
		if @collection.length < MAX_SLOTS
			@ui.addBtn.show()
		else
			@ui.addBtn.hide()

	addItem: ->
		unless @collection.length >=  MAX_SLOTS
			console.log 'Adding new item to schedule'
			@collection.add new Backbone.Model
				s: 1
				h: 12
				m: 0
				q: 1

	handleDeleteChild: (cv)->
		console.log 'Removing child from collection'
		@collection.remove cv.model

	setArduinoSchedule: ->
		schedule = @collection.toJSON()

		try
			await RequestUtils.setSchedule JSON.stringify(schedule)
			Toastr.success "Schedule successfully updated!"
		catch err
			console.error 'Error updating schedule in Arduino:', err
			Toastr.error 'Error updating schedule in Arduino'
