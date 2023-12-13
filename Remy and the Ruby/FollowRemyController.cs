using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FollowPlayer : MonoBehaviour
{
    public GameObject remy;
    private Vector3 offset = new Vector3(0, 2, -5);

    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void LateUpdate()
    {
        if (Input.GetKey(KeyCode.UpArrow) || Input.GetKey(KeyCode.DownArrow))
        {
            //Offset the camera behind the player by adding the player position.
            transform.position = remy.transform.position + offset;
        }
    }
}
