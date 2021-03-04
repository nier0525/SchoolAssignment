using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class IceShot : MonoBehaviour
{
    GameObject Player;

    float angle;

    // Start is called before the first frame update
    void Start()
    {
        Player = GameObject.Find("Player");
        transform.eulerAngles = new Vector3(0, 0, -GetAngle(transform.position, Player.transform.position) - 90);
        Destroy(gameObject, 6);
    }

    float GetAngle(Vector3 a, Vector3 b)
    {
        float dx = b.x - a.x;
        float dy = b.y - a.y;

        float rad = Mathf.Atan2(dx, dy);
        float degree = rad * Mathf.Rad2Deg;

        return degree;
    }

    // Update is called once per frame
    void Update()
    {
        transform.Translate(-3f * Time.deltaTime, 0, 0);
    }
}
