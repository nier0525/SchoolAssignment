using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DestoryPrefab : MonoBehaviour {

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		if(this.gameObject.name == "fireball_hit(Clone)" || this.gameObject.name == "meteor_hit(Clone)")
        {
            Destroy(this.gameObject, 0.51f);
        }
        if(this.gameObject.name == "firebolt_hit(Clone)")
        {
            Destroy(this.gameObject, 0.5f);
        }
        if(this.gameObject.name == "firecast(Clone)")
        {
            Destroy(this.gameObject, 0.52f);
        }
        if(this.gameObject.name == "icecast(Clone)")
        {
            Destroy(this.gameObject, 0.45f);
        }
        if(this.gameObject.name == "icestorm_impact(Clone)")
        {
            Destroy(this.gameObject, 0.49f);
        }
        if (this.gameObject.name == "manashieldCast(Clone)")
        {
            Destroy(this.gameObject, 0.2f);
        }
    }
}
