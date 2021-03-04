using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HepstoStatus : MonoBehaviour
{
    public int hp;

    public int Att;
    public int Def;

    public int exp;

    // Start is called before the first frame update
    void init()
    {
        hp = 8000;
        Att = 100;
        Def = 10;
        exp = 7000;
    }

    private void Awake()
    {
        init();
    }
}
