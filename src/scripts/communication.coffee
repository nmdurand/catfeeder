import axios from 'axios'
import $ from 'jquery'

$('#getData').on 'click', ->
	try
		response = await axios.get 'http://192.168.1.36/state/get'
		console.debug 'Got response:', response.data
		$('#textField').val response.data.msg
	catch err
		console.debug err

$('#setData').on 'click', ->
	try
		newState = $('#textField').val()
		response = await axios.get 'http://192.168.1.36/state/set',
			params:
				msg: newState
		console.debug 'Got response:', response.data
		$('#textField').val ""
	catch err
		console.debug err