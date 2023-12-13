using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AnimationStateController : MonoBehaviour
{
    Animator animator;
    int isWalkingHash;
    int isRunningHash;
    int isWalkingBackwardsHash;
    int isRunningBackwardsHash;
    int isWalkingLeftHash;
    int isWalkingRightHash;

    // Start is called before the first frame update
    void Start()
    {
        animator = GetComponent<Animator>();
        isWalkingHash = Animator.StringToHash("isWalking");
        isRunningHash = Animator.StringToHash("isRunning");
        isWalkingBackwardsHash = Animator.StringToHash("isWalkingBackwards");
        isRunningBackwardsHash = Animator.StringToHash("isRunningBackwards");
        isWalkingLeftHash = Animator.StringToHash("isWalkingLeft");
        isWalkingRightHash = Animator.StringToHash("isWalkingRight");
    }

    // Update is called once per frame
    void Update()
    {
        bool isWalking = animator.GetBool(isWalkingHash);
        bool isRunning = animator.GetBool(isRunningHash);
        bool isWalkingBackwards = animator.GetBool(isWalkingBackwardsHash);
        bool isRunningBackwards = animator.GetBool(isRunningBackwardsHash);
        bool isWalkingLeft = animator.GetBool(isWalkingLeftHash);
        bool isWalkingRight = animator.GetBool(isWalkingRightHash);

        bool forwardPressed = Input.GetKey(KeyCode.UpArrow);
        bool runPressed = Input.GetKey(KeyCode.LeftShift);
        bool backwardPressed = Input.GetKey(KeyCode.DownArrow);
        bool leftPressed = Input.GetKey(KeyCode.LeftArrow);
        bool rightPressed = Input.GetKey(KeyCode.RightArrow);

        if (forwardPressed && !runPressed)
        {
            animator.SetBool(isWalkingHash, true);
        }

        if (!forwardPressed)
        {
            animator.SetBool(isWalkingHash, false);
        }

        if (runPressed && forwardPressed)
        {
            animator.SetBool(isWalkingHash, false);
            animator.SetBool(isRunningBackwardsHash, false);
            animator.SetBool(isRunningHash, true);
        }

        if (runPressed && backwardPressed)
        {
            animator.SetBool(isWalkingBackwardsHash, false);
            animator.SetBool(isRunningHash, false);
            animator.SetBool(isRunningBackwardsHash, true);
        }

        if (runPressed && !forwardPressed && !backwardPressed)
        {
            animator.SetBool(isRunningHash, false);
            animator.SetBool(isRunningBackwardsHash, false);
        }

        if ((!runPressed && !forwardPressed) || (!runPressed && !backwardPressed))
        {
            animator.SetBool(isRunningHash, false);
            animator.SetBool(isRunningBackwardsHash, false);
        }

        if (backwardPressed && !runPressed)
        {
            animator.SetBool(isWalkingBackwardsHash, true);
        }

        if (!backwardPressed)
        {
            animator.SetBool(isWalkingBackwardsHash, false);
        }

        if (leftPressed)
        {
            animator.SetBool(isWalkingLeftHash, true);
        }

        if (!leftPressed)
        {
            animator.SetBool(isWalkingLeftHash, false);
        }

        if (leftPressed && runPressed && forwardPressed)
        {
            animator.SetBool(isWalkingLeftHash, false);
            animator.SetBool(isRunningHash, true);
        }

        if (leftPressed && runPressed && backwardPressed)
        {
            animator.SetBool(isWalkingLeftHash, false);
            animator.SetBool(isRunningBackwardsHash, true);
        }

        if (rightPressed)
        {
            animator.SetBool(isWalkingRightHash, true);
        }

        if (!rightPressed)
        {
            animator.SetBool(isWalkingRightHash, false);
        }

        if (rightPressed && runPressed && forwardPressed)
        {
            animator.SetBool(isWalkingRightHash, false);
            animator.SetBool(isRunningHash, true);
        }

        if (rightPressed && runPressed && backwardPressed)
        {
            animator.SetBool(isWalkingRightHash, false);
            animator.SetBool(isRunningBackwardsHash, true);
        }
    }
}
