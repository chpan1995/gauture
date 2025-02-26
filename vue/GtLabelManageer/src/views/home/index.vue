<template>
  <div>
    <el-card>
      <div class="box">
        <div class="avatar"></div>
        <div class="bottom">
          <h3 class="title">{{ getTime() }}好呀{{ userStore.username }}</h3>
          <p class="subtitle">高哲标注后台管理系统</p>
        </div>
      </div>
    </el-card>
    <div class="bottoms">
      <div class="welcome"></div>
    </div>
  </div>
</template>

<script setup lang="ts">
//引入用户相关的仓库,获取当前用户的头像、昵称
import useUserStore from "@/store/modules/user";
//获取存储用户信息的仓库对象
let userStore = useUserStore();

const getTime = () => {
  let message = "";
  //通过内置构造函数Date
  const hours = new Date().getHours();
  //情况的判断
  if (hours <= 9) {
    message = "早上";
  } else if (hours <= 12) {
    message = "上午";
  } else if (hours <= 18) {
    message = "下午";
  } else {
    message = "晚上";
  }
  return message;
};
</script>

<style scoped lang="scss">
.box {
  display: flex;

  .avatar {
    width: 100px;
    height: 100px;
    border-radius: 50%;
    background: url("@/assets/avatar.png") no-repeat;
    background-position: center;
    /* 背景图片居中显示 */
    background-size: cover;
  }

  .bottom {
    margin-left: 20px;

    .title {
      font-size: 30px;
      font-weight: 900;
      margin-bottom: 30px;
    }

    .subtitle {
      font-style: italic;
      color: skyblue;
    }
  }
}
.bottoms {
  margin-top: 10px;
  display: flex;
  justify-content: center;

  .welcome {
    background: url("@/assets/homeback.png") no-repeat;
    background-position: center;
    /* 背景图片居中显示 */
    background-size: cover;
    width: 600px;
    height: 360px;
  }
}
</style>