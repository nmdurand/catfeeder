import axios from 'axios'
import $ from 'jquery'

$('#getData').on 'click', ->
	try
		response = await axios.get 'http://192.168.1.36/data/get'
		console.log 'Data response:', response.data
		$('#textField').val JSON.stringify(response.data)
	catch err
		console.log err

$('#setData').on 'click', ->
	try
		data = $('#textField').val()
		response = await axios.get 'http://192.168.1.36/data/set',
			params:
				data: data
		console.log 'Response trying to set data:', response
	catch err
		console.log err
