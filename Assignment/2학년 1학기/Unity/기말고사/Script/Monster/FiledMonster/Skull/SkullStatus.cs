﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SkullStatus : MonoBehaviour
{
    public int hp;

    public int Att;
    public int Def;

    public int exp;

    // Start is called before the first frame update
    void init()
    {
        hp = 1000;
        Att = 15;
        Def = 10;
        exp = 100;
    }

    private void Awake()
    {
        init();
    }
}
