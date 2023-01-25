import json
from flask import Flask, jsonify, request
from polygon3d import Point3D, Polygon3D

HELP_TEXT = """
    Check whether a 3-dimensional point lies within or outside of a 3-dimensional polygon.
    Points on an edge or vertex will return false. 
    Send a POST request to the /contains endpoint with the following format:
    {
        "point": [0.5, 0.5, 0.0],
        "polygon_vertices":
            [
                [0.0, 1.0, 0.0],
                [1.0, 1.0, 0.0],
                [1.0, 0.0, 0.0],
                [0.0, 0.0, 0.0]
            ]
    }
    The 'point' field should contain a 3-dimensional point, and the 'polygon_vertices' field
    should contain a list of at least three 3-dimensional points that form a valid polygon.
    Example successful response for the above request:
    {
        "point": [0.5, 0.5, 0.0],
        "polygon_contains_point": true,
        "polygon_vertices": [
            [0.0, 1.0, 0.0],
            [1.0, 1.0, 0.0],
            [1.0, 0.0, 0.0],
            [0.0, 0.0, 0.0]
        ]
    }
"""

app = Flask(__name__)

@app.route('/', methods=['GET'])
def home():
    return HELP_TEXT

@app.route('/contains', methods=['POST'])
def contains():
    try:
        request_data = request.get_json(force=True)
        polygon_vertices = request_data['polygon_vertices']
        polygon = Polygon3D(
            [
                Point3D(vertex[0], vertex[1], vertex[2])
                for vertex in polygon_vertices
            ]
        )
        return jsonify(
            {
                'polygon_vertices': request_data['polygon_vertices'],
                'point': request_data['point'],
                'polygon_contains_point': polygon.contains(
                    Point3D(
                        request_data['point'][0],
                        request_data['point'][1],
                        request_data['point'][2],
                    )
                )
            }
        )
    except Exception as e:
        return json.dumps({'status': "Error", 'message': '{}'.format(e)}), 403


if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0')
