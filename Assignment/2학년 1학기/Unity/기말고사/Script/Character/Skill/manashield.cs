using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class manashield : MonoBehaviour {

    [HideInInspector] public int damage; // 쉴드량
    public static int max_damage;
    private void Start()
    {
        damage = PlayerState.mpp / 15;
        max_damage = damage;
    }
}
