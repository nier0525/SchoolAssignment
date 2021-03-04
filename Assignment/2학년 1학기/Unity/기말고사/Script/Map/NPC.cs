using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class NPC : MonoBehaviour
{
    bool state = false;

    // Start is called before the first frame update
    void Start()
    {

    }

    public void MouseIn()
    {
        state = true;
    }

    public void MouseOut()
    {
        state = false;
    }


    public bool MouseState()
    {
        return state;
    }

   //  Update is called once per frame
    void Update()
    {

    }
}
