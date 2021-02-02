import axios from 'axios'
import $ from 'jquery'

$('#getData').on 'click', ->
	try
		response = await axios.get 'http://192.168.1.36/state/get'
		console.log 'Data response:', response.state
		$('#textField').val JSON.stringify(response.state)
	catch err
		console.log err

$('#setData').on 'click', ->
	try
		data = $('#textField').val()
		response = await axios.get 'http://192.168.1.36/state/set',
			params:
				state: state
		console.log 'Response trying to set state:', response
	catch err
		console.log err
