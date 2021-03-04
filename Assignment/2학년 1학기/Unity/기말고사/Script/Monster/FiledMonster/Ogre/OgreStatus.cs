using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class OgreStatus : MonoBehaviour
{
    public int hp;

    public int Att;
    public int Def;

    public int exp;

    // Start is called before the first frame update
    void init()
    {
        hp = 500;
        Att = 20;
        Def = 10;
        exp = 500;
    }

    private void Awake()
    {
        init();
    }
}
