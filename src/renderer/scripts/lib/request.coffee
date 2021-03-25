import axios from 'axios'
import $ from 'jquery'
mdnsResolver = require 'mdns-resolver'

HOST_NAME = 'catfeeder.local'

export default RequestUtils =
	connectServer: ->
		console.debug 'Trying to connect to catfeeder server...'
		@serverIp = await mdnsResolver.resolve HOST_NAME, 'A'
		console.log 'Found catfeeder server ip:', @serverIp

	getSchedule: ->
		try
			response = await axios.get "http://#{@serverIp}/schedule/get"
			console.debug 'Received response:', response.data
			response.data
		catch err
			console.debug err

	setSchedule: (schedule)->
		try
			response = await axios.get "http://#{@serverIp}/schedule/set",
				params:
					schedule: schedule
			console.debug 'Received response:', response.data
		catch err
			console.debug err
