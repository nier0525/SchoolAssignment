using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Curser : MonoBehaviour
{
    Vector2 targetpos;
    public Texture2D[] cursorBase = new Texture2D[8];
    public Texture2D[] cursorGrip = new Texture2D[8];
    public Texture2D[] cursorWait = new Texture2D[8];

    public bool GripCheak = false;
    public bool Waiting = false;

    float index, num;
    bool down = false;
    // Start is called before the first frame update
    void Start()
    {
        // StartCoroutine("SpriteActive");
        index = 0f;
        num = 7f;
    }

    /*
    //IEnumerator SpriteActive()
    //{
    //    yield return new WaitForEndOfFrame();

    //    //targetpos.x = cursorBase.width / 15f;
    //    //targetpos.y = cursorBase.height / 17f;

    //    //if (GripCheak)
    //    //{
    //    //    while (GripCheak)
    //    //    {
    //    //        if (index >= num) index = 0;
    //    //        Cursor.SetCursor(cursorGrip[index], targetpos, CursorMode.Auto);
    //    //        index++;
    //    //        yield return new WaitForSeconds(0.1f);
    //    //    }
    //    //}
    //    //else
    //    //{
    //    //    Cursor.SetCursor(cursorBase, targetpos, CursorMode.Auto);
    //    //}
    //}
    */

    // Update is called once per frame
    void Update()
    {


        index += 0.15f;
        if (index >= num) index = 0;

        targetpos.x = cursorBase[(int)index].width / 15f;
        targetpos.y = cursorBase[(int)index].height / 17f;

        if (GripCheak)
        {
            Cursor.SetCursor(cursorGrip[(int)index], targetpos, CursorMode.Auto);
        }

        if (Waiting)
        {
            Cursor.SetCursor(cursorWait[(int)index], targetpos, CursorMode.Auto);
        }

        else
        {
            Cursor.SetCursor(cursorBase[(int)index], targetpos, CursorMode.Auto);
        }
    }
}
