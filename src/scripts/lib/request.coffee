import axios from 'axios'
import $ from 'jquery'

SERVER_URL = 'http://192.168.1.26/'

export default RequestUtils =
	getSchedule: ->
		try
			response = await axios.get(SERVER_URL + 'schedule/get')
			console.debug 'Received response:', response.data
			response.data
		catch err
			console.debug err

	setSchedule: (schedule)->
		try
			response = await axios.get (SERVER_URL + 'schedule/set'),
				params: schedule
					# msg: newState
			console.debug 'Received response:', response.data
		catch err
			console.debug err
