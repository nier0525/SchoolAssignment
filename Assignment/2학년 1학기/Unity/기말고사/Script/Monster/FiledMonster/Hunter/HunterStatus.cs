using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HunterStatus : MonoBehaviour
{
    public int hp;

    public int Att;
    public int Def;

    public int exp;

    // Start is called before the first frame update
    void init()
    {
        hp = 200;
        Att = 10;
        Def = 10;
        exp = 200;
    }

    private void Awake()
    {
        init();
    }
}
