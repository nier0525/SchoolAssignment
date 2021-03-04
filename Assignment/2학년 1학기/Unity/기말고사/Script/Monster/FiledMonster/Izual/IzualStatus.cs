using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class IzualStatus : MonoBehaviour
{
    public int hp;

    public int Att;
    public int Def;

    public int exp;

    // Start is called before the first frame update
    void init()
    {
        hp = 5000;
        Att = 80;
        Def = 10;
        exp = 5000;
    }

    private void Awake()
    {
        init();
    }
}
